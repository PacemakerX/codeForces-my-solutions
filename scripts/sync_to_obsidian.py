#!/usr/bin/env python3
"""
Sync CF solutions → Obsidian vault (one-way, additive only).

Tags in notes are the exact CF API tag strings (e.g. "constructive algorithms",
"binary search"). The Related section links to the corresponding pattern files.
Difficulty is the actual CF rating (e.g. "1200") not a letter heuristic.

CF data is cached in logs/cf_problems_cache.json (refreshed every 7 days).

Usage:
    python3 scripts/sync_to_obsidian.py                   # normal sync
    python3 scripts/sync_to_obsidian.py --dry-run          # preview only
    python3 scripts/sync_to_obsidian.py --verbose          # show skipped too
    python3 scripts/sync_to_obsidian.py --rewrite-code     # update Code block in all notes
    python3 scripts/sync_to_obsidian.py --retag            # update tags+difficulty in all notes
    python3 scripts/sync_to_obsidian.py --refresh-cache    # force re-fetch CF problem data
"""
import json
import re
import sys
import logging
import urllib.request
from datetime import date, datetime, timezone
from pathlib import Path

# ---------------------------------------------------------------------------
# Paths & constants
# ---------------------------------------------------------------------------
REPO_ROOT          = Path(__file__).resolve().parent.parent
SOLUTIONS_DIR      = REPO_ROOT / "solutions_codForces"
VAULT_DIR          = Path("/home/sparsh/Documents/vault/01-problems")
LOG_FILE           = REPO_ROOT / "logs" / "sync.log"
CF_CACHE_FILE      = REPO_ROOT / "logs" / "cf_problems_cache.json"
CF_DATES_CACHE     = REPO_ROOT / "logs" / "cf_submission_dates_cache.json"

CF_HANDLE          = "PacemakerX"
CF_PROBLEMS_URL    = "https://codeforces.com/api/problemset.problems"
CF_STATUS_URL      = "https://codeforces.com/api/user.status?handle={handle}&from=1&count=10000"
CACHE_MAX_AGE_DAYS = 7

DRY_RUN       = "--dry-run"       in sys.argv
VERBOSE       = "--verbose"       in sys.argv
REWRITE_CODE  = "--rewrite-code"  in sys.argv
RETAG         = "--retag"         in sys.argv
REFRESH_CACHE = "--refresh-cache" in sys.argv

# ---------------------------------------------------------------------------
# Logging
# ---------------------------------------------------------------------------
logger = logging.getLogger("sync")
logger.setLevel(logging.DEBUG)

_fmt = logging.Formatter("%(asctime)s  %(levelname)-7s  %(message)s",
                         datefmt="%Y-%m-%d %H:%M:%S")

LOG_FILE.parent.mkdir(exist_ok=True)
_fh = logging.FileHandler(LOG_FILE, encoding="utf-8")
_fh.setFormatter(_fmt)
_fh.setLevel(logging.DEBUG)

_sh = logging.StreamHandler(sys.stdout)
_sh.setFormatter(_fmt)
_sh.setLevel(logging.DEBUG if VERBOSE else logging.INFO)

logger.addHandler(_fh)
logger.addHandler(_sh)

# ---------------------------------------------------------------------------
# CF tag → pattern file mapping
# Keys are exact CF tag strings. Values are pattern file stems (02-patterns/).
# CF tags not listed here are stored in the note but produce no Related link.
# ---------------------------------------------------------------------------
CF_TAG_TO_PATTERN: dict[str, str] = {
    "dp":                       "DP",
    "graphs":                   "Graphs",
    "trees":                    "Graphs",
    "dfs and similar":          "Graphs",
    "shortest paths":           "Graphs",
    "flows":                    "Graphs",
    "dsu":                      "DataStructures",
    "binary search":            "BinarySearch",
    "ternary search":           "BinarySearch",
    "data structures":          "DataStructures",
    "bitmasks":                 "BitManipulation",
    "math":                     "Math",
    "number theory":            "Math",
    "probabilities":            "Math",
    "matrices":                 "Math",
    "chinese remainder theorem": "Math",
    "strings":                  "StringAlgo",
    "string suffix structures": "StringAlgo",
    "hashing":                  "StringAlgo",
    "two pointers":             "TwoPointers",
    "sliding window":           "SlidingWindow",
    "greedy":                   "Greedy",
    "sortings":                 "Greedy",
    "implementation":           "Greedy",
    "brute force":              "Greedy",
    "combinatorics":            "Combinatorics",
    "constructive algorithms":  "ConstructiveAlgo",
    "games":                    "Games",
    # "geometry", "*special" — no pattern file yet
}


def to_obsidian_tag(cf_tag: str) -> str:
    """Convert CF tag string to a valid Obsidian tag (no spaces allowed)."""
    return cf_tag.replace(" ", "-")

# ---------------------------------------------------------------------------
# CF problem cache
# ---------------------------------------------------------------------------
def _cache_is_fresh() -> bool:
    if not CF_CACHE_FILE.exists():
        return False
    mtime = datetime.fromtimestamp(CF_CACHE_FILE.stat().st_mtime, tz=timezone.utc)
    return (datetime.now(tz=timezone.utc) - mtime).days < CACHE_MAX_AGE_DAYS

def fetch_cf_problems(force: bool = False) -> dict[str, dict]:
    """Return {"{contestId}_{index}": {tags, rating, name}} from cache or API."""
    if not force and _cache_is_fresh():
        logger.debug("CF cache is fresh — loading from %s", CF_CACHE_FILE.name)
        return json.loads(CF_CACHE_FILE.read_text(encoding="utf-8"))

    logger.info("Fetching CF problem list from API...")
    try:
        with urllib.request.urlopen(CF_API_URL, timeout=15) as resp:
            data = json.loads(resp.read())
    except Exception as e:
        logger.warning("CF API fetch failed: %s — falling back to cache", e)
        return json.loads(CF_CACHE_FILE.read_text(encoding="utf-8")) if CF_CACHE_FILE.exists() else {}

    if data.get("status") != "OK":
        logger.warning("CF API status=%s — falling back to cache", data.get("status"))
        return json.loads(CF_CACHE_FILE.read_text(encoding="utf-8")) if CF_CACHE_FILE.exists() else {}

    cache: dict[str, dict] = {}
    for p in data["result"]["problems"]:
        cid = str(p.get("contestId", ""))
        idx = p.get("index", "").upper()
        if not cid or not idx:
            continue
        cache[f"{cid}_{idx}"] = {
            "name":   p.get("name", ""),
            "rating": p.get("rating"),
            "tags":   p.get("tags", []),
        }

    CF_CACHE_FILE.write_text(json.dumps(cache, ensure_ascii=False), encoding="utf-8")
    logger.info("CF cache updated — %d problems → %s", len(cache), CF_CACHE_FILE.name)
    return cache


# ---------------------------------------------------------------------------
# Submission dates cache  {str(submission_id): "YYYY-MM-DD"}
# Fetched from user.status — gives the exact date the AC was submitted.
# ---------------------------------------------------------------------------
def fetch_submission_dates(force: bool = False) -> dict[str, str]:
    """Return {sub_id_str: "YYYY-MM-DD"} for all accepted submissions."""
    if not force and CF_DATES_CACHE.exists():
        mtime = datetime.fromtimestamp(CF_DATES_CACHE.stat().st_mtime, tz=timezone.utc)
        if (datetime.now(tz=timezone.utc) - mtime).days < CACHE_MAX_AGE_DAYS:
            logger.debug("Submission dates cache is fresh — loading from %s", CF_DATES_CACHE.name)
            return json.loads(CF_DATES_CACHE.read_text(encoding="utf-8"))

    logger.info("Fetching submission dates from CF API (handle=%s)...", CF_HANDLE)
    url = CF_STATUS_URL.format(handle=CF_HANDLE)
    try:
        with urllib.request.urlopen(url, timeout=20) as resp:
            data = json.loads(resp.read())
    except Exception as e:
        logger.warning("CF user.status fetch failed: %s — falling back to cache", e)
        return json.loads(CF_DATES_CACHE.read_text(encoding="utf-8")) if CF_DATES_CACHE.exists() else {}

    if data.get("status") != "OK":
        logger.warning("CF user.status status=%s — falling back to cache", data.get("status"))
        return json.loads(CF_DATES_CACHE.read_text(encoding="utf-8")) if CF_DATES_CACHE.exists() else {}

    dates: dict[str, str] = {}
    for sub in data["result"]:
        if sub.get("verdict") != "OK":
            continue
        sub_id = str(sub["id"])
        ts = sub.get("creationTimeSeconds")
        if ts:
            dates[sub_id] = datetime.fromtimestamp(ts, tz=timezone.utc).strftime("%Y-%m-%d")

    CF_DATES_CACHE.write_text(json.dumps(dates, ensure_ascii=False), encoding="utf-8")
    logger.info("Submission dates cached — %d accepted submissions → %s",
                len(dates), CF_DATES_CACHE.name)
    return dates


def lookup_cf(contest_id: str, prob_letter: str,
              cf_db: dict) -> tuple[list[str], str]:
    """
    Returns (cf_tags, difficulty).
    cf_tags are exact CF strings (e.g. ["binary search", "greedy"]).
    Difficulty is the numeric CF rating string or a letter-based fallback.
    """
    entry = cf_db.get(f"{contest_id}_{prob_letter.upper()}")
    if not entry:
        return [], diff_from_letter(prob_letter)
    rating = entry.get("rating")
    diff = str(rating) if rating else diff_from_letter(prob_letter)
    return entry.get("tags", []), diff


# ---------------------------------------------------------------------------
# Fallback tag inference (used only when CF API has no data for this problem)
# Returns tags in CF-style lowercase to stay consistent.
# ---------------------------------------------------------------------------
_TAG_RULES_FALLBACK = [
    ("graphs",                  [r'\bqueue<', r'\bbfs\b', r'\bdfs\b', r'adj\[',
                                  r'adj\.push', r'adj\.emplace', r'visited\[', r'dist\[']),
    ("dp",                      [r'\bdp\[', r'\bmemo\[', r'\bndp\[', r'\bf\[.*\]\[', r'\bdp2\[']),
    ("binary search",           [r'while\s*\(\s*lo\s*<\s*hi\s*\)',
                                  r'while\s*\(\s*low\s*<=?\s*high\s*\)',
                                  r'lo\s*\+\s*\(hi', r'low\s*\+\s*\(high',
                                  r'\blower_bound\s*\(', r'\bupper_bound\s*\(']),
    ("data structures",         [r'\bDSU\b', r'\bBIT\b', r'\bfenwick\b', r'\bSegTree\b',
                                  r'\bseg\[', r'\bstack<', r'\bordered_set\b', r'\bdeque<']),
    ("bitmasks",                [r'__builtin_popcount', r'\(1\s*<<', r'\(1LL\s*<<',
                                  r'__builtin_ctz', r'__builtin_clz', r'\bmask\b']),
    ("math",                    [r'\bgcd\(', r'\blcm\(', r'\bbinpow\(', r'\bmod_pow\(',
                                  r'\bsieve\b', r'\bprime\b', r'\bfact\[', r'\binv_fact\[']),
    ("strings",                 [r'\blps\[', r'z_func', r'z\[0\]\s*=\s*n', r'kmp\b',
                                  r'\.find\s*\(', r'\.substr\s*\(']),
    ("two pointers",            [r'\bl\s*=\s*0.*\br\s*=', r'l\+\+.*r--']),
    ("greedy",                  [r'\bsort\s*\(']),
    ("combinatorics",           [r'\binv_fact\[', r'\bnCr\b', r'\bcatalan\b',
                                  r'fact\[.*\]\s*%.*inv_fact\[']),
    ("constructive algorithms", [r'interleave', r'construct\b']),
]

def infer_tags_fallback(solve_body: str) -> list[str]:
    tags = [tag for tag, pats in _TAG_RULES_FALLBACK
            if any(re.search(p, solve_body) for p in pats)]
    return tags if tags else ["greedy"]


# ---------------------------------------------------------------------------
# Build Related links from CF tags
# ---------------------------------------------------------------------------
def build_related(cf_tags: list[str]) -> str:
    seen: set[str] = set()
    links: list[str] = []
    for tag in cf_tags:
        pattern = CF_TAG_TO_PATTERN.get(tag)
        if pattern and pattern not in seen:
            links.append(f"- [[02-patterns/{pattern}]]")
            seen.add(pattern)
    return "\n".join(links)


# ---------------------------------------------------------------------------
# Extract code from .cpp file
# ---------------------------------------------------------------------------
def extract_code_block(content: str) -> str:
    return content.strip()

def extract_solve(content: str) -> str:
    """solve() body only — used for fallback tag inference."""
    m = re.search(r'//[-/]+//\s*\n', content)
    after = content[m.end():] if m else content
    m2 = re.search(r'(void solve\s*\(\s*\).*?)(?=\nint main\s*\()', after, re.DOTALL)
    if m2:
        return m2.group(1).strip()
    idx = after.find('\nint main')
    return after[:idx].strip() if idx != -1 else after.strip()


# ---------------------------------------------------------------------------
# Difficulty heuristic from problem letter (fallback only)
# ---------------------------------------------------------------------------
def diff_from_letter(letter: str) -> str:
    base = letter.upper().rstrip('12')
    return {'A': '~800', 'B': '~1100', 'C': '~1400',
            'D': '~1700', 'E': '~2000'}.get(base, '~2200')


# ---------------------------------------------------------------------------
# Build the markdown note content
# ---------------------------------------------------------------------------
def build_note(sub_id: str, contest_id: str, prob_letter: str,
               code_block: str, cf_tags: list[str], diff: str,
               sub_date: str | None = None) -> str:
    prob_id   = f"{contest_id}{prob_letter}"
    cf_url    = f"https://codeforces.com/contest/{contest_id}/problem/{prob_letter}"
    tags_yaml = ", ".join(f'"{to_obsidian_tag(t)}"' for t in cf_tags)
    related   = build_related(cf_tags)
    today     = sub_date or date.today().isoformat()

    return f"""---
title: "CF {prob_id}"
platform: "CF"
problem-id: "{prob_id}"
difficulty: "{diff}"
tags: [{tags_yaml}]
date: {today}
result: "AC"
time-to-solve: ""
attempts: 1
link: "{cf_url}"
submission-id: "{sub_id}"
---

# CF {prob_id}

## Problem Summary
<!-- Fill in: what does the problem ask? What are key constraints? -->

## Observations
1.

## Approach


## Complexity
- Time: `O()`
- Space: `O()`

## Code
```cpp
{code_block}
```

## What Went Wrong


## Key Takeaway


## Related
{related}
"""


# ---------------------------------------------------------------------------
# Similarity check
# ---------------------------------------------------------------------------
_TEMPLATE_TOKENS = {
    "int", "ll", "vl", "vi", "vvi", "vvl", "pii", "pll",
    "n", "m", "i", "j", "k", "t", "x", "y",
    "cin", "cout", "nline", "endl",
    "forn", "forr", "ford", "all", "rall", "sz", "pb", "mp",
    "F", "S", "yes", "no", "MOD", "INF",
    "void", "solve", "return", "if", "else", "while", "for",
    "auto", "sort", "min", "max", "abs", "swap",
    "true", "false", "bool", "string", "char",
}

def _signature_tokens(body: str) -> set[str]:
    return set(re.findall(r'[A-Za-z_]\w{2,}', body)) - _TEMPLATE_TOKENS

def _same_problem(body_a: str, body_b: str) -> bool:
    la, lb = body_a.count('\n') + 1, body_b.count('\n') + 1
    if la and lb and max(la, lb) / min(la, lb) > 2.0:
        return False
    sa, sb = _signature_tokens(body_a), _signature_tokens(body_b)
    if not sa and not sb:
        return True
    if not sa or not sb:
        return False
    return len(sa & sb) / min(len(sa), len(sb)) >= 0.4


# ---------------------------------------------------------------------------
# Collect .cpp files
# ---------------------------------------------------------------------------
def collect_problems() -> list[tuple]:
    grouped: dict[str, list[tuple]] = {}
    for f in SOLUTIONS_DIR.glob("*.cpp"):
        m = re.match(r'^(\d+)_(\d+)_([A-Za-z0-9]+)\.cpp$', f.name)
        if not m:
            continue
        sub_id, contest_id, prob_letter = m.groups()
        prob_letter = prob_letter.upper()
        grouped.setdefault(f"{contest_id}_{prob_letter}",
                           []).append((int(sub_id), contest_id, prob_letter, f))

    entries: list[tuple] = []
    for key, subs in grouped.items():
        subs.sort(key=lambda x: x[0], reverse=True)
        contest_id, prob_letter = subs[0][1], subs[0][2]

        if len(subs) == 1:
            sub_id, _, _, fpath = subs[0]
            entries.append((f"CF_{contest_id}_{prob_letter}",
                            str(sub_id), contest_id, prob_letter, fpath))
            continue

        newest, oldest = subs[0], subs[-1]
        try:
            body_new = extract_solve(newest[3].read_text(errors="replace"))
            body_old = extract_solve(oldest[3].read_text(errors="replace"))
        except OSError:
            entries.append((f"CF_{contest_id}_{prob_letter}",
                            str(newest[0]), contest_id, prob_letter, newest[3]))
            continue

        entries.append((f"CF_{contest_id}_{prob_letter}",
                        str(newest[0]), contest_id, prob_letter, newest[3]))

        if not _same_problem(body_new, body_old):
            logger.warning(
                "CONFLICT %s_%s — submissions %s and %s have different solutions. "
                "Creating _old note for the earlier one.",
                contest_id, prob_letter, newest[0], oldest[0]
            )
            entries.append((f"CF_{contest_id}_{prob_letter}_old",
                            str(oldest[0]), contest_id, prob_letter, oldest[3]))

    return entries


def build_note_map() -> dict[str, tuple[str, str, str, Path]]:
    """Returns {stem: (sub_id, contest_id, prob_letter, fpath)}"""
    return {stem: (sub_id, cid, letter, fpath)
            for stem, sub_id, cid, letter, fpath in collect_problems()}


# ---------------------------------------------------------------------------
# Rewrite Code block
# ---------------------------------------------------------------------------
_CODE_BLOCK_RE = re.compile(r'(## Code\n```cpp\n).*?(```)', re.DOTALL)

def rewrite_code_in_note(note_path: Path, new_code: str) -> bool:
    text = note_path.read_text(encoding="utf-8")
    new_text, n = _CODE_BLOCK_RE.subn(rf'\g<1>{new_code}\n\g<2>', text, count=1)
    if n == 0 or new_text == text:
        return False
    note_path.write_text(new_text, encoding="utf-8")
    return True


# ---------------------------------------------------------------------------
# Retag — update tags + difficulty in YAML frontmatter + Related section
# ---------------------------------------------------------------------------
_TAGS_RE = re.compile(r'^(tags:\s*)\[.*?\]', re.MULTILINE)
_DIFF_RE = re.compile(r'^(difficulty:\s*)"[^"]*"', re.MULTILINE)
_DATE_RE = re.compile(r'^(date:\s*)\S+', re.MULTILINE)
_RELATED_RE = re.compile(r'(## Related\n)(.*?)(\n##|\Z)', re.DOTALL)

def retag_note(note_path: Path, cf_tags: list[str], diff: str,
               sub_date: str | None = None) -> bool:
    """Update tags, difficulty, date in YAML frontmatter, and Related wikilinks."""
    text = note_path.read_text(encoding="utf-8")
    tags_str = ", ".join(f'"{to_obsidian_tag(t)}"' for t in cf_tags)
    new_text = _TAGS_RE.sub(rf'\g<1>[{tags_str}]', text, count=1)
    new_text = _DIFF_RE.sub(rf'\g<1>"{diff}"', new_text, count=1)
    if sub_date:
        new_text = _DATE_RE.sub(rf'\g<1>{sub_date}', new_text, count=1)
    # Rewrite Related section with all pattern links derived from current tags
    related_body = build_related(cf_tags)
    def _replace_related(m: re.Match) -> str:
        return m.group(1) + related_body + "\n" + m.group(3)
    new_text = _RELATED_RE.sub(_replace_related, new_text, count=1)
    if new_text == text:
        return False
    note_path.write_text(new_text, encoding="utf-8")
    return True


# ---------------------------------------------------------------------------
# Main sync
# ---------------------------------------------------------------------------
def sync(cf_db: dict, dates: dict) -> tuple[int, int, int]:
    if not VAULT_DIR.exists():
        logger.error("Vault directory not found: %s", VAULT_DIR)
        sys.exit(1)

    entries = collect_problems()
    logger.info("Found %d entries to sync", len(entries))
    created = skipped = errors = 0

    for note_stem, sub_id, contest_id, prob_letter, fpath in sorted(entries):
        note_name = f"{note_stem}.md"
        note_path = VAULT_DIR / note_name

        if note_path.exists():
            logger.debug("SKIP  %s (note exists)", note_name)
            skipped += 1
            continue

        try:
            content = fpath.read_text(errors="replace")
        except OSError as e:
            logger.error("ERROR reading %s: %s", fpath.name, e)
            errors += 1
            continue

        cf_tags, diff = lookup_cf(contest_id, prob_letter, cf_db)
        if not cf_tags:
            cf_tags = infer_tags_fallback(extract_solve(content))

        sub_date = dates.get(sub_id)
        code_block = extract_code_block(content)
        note = build_note(sub_id, contest_id, prob_letter, code_block, cf_tags, diff, sub_date)

        if DRY_RUN:
            logger.info("DRY   %s  tags=%s  diff=%s  date=%s",
                        note_name, cf_tags, diff, sub_date or "today")
        else:
            note_path.write_text(note, encoding="utf-8")
            logger.info("NEW   %s  tags=%s  diff=%s  date=%s",
                        note_name, cf_tags, diff, sub_date or "today")
        created += 1

    return created, skipped, errors


# ---------------------------------------------------------------------------
# Rewrite-code mode
# ---------------------------------------------------------------------------
def rewrite_all_code() -> tuple[int, int, int]:
    if not VAULT_DIR.exists():
        logger.error("Vault directory not found: %s", VAULT_DIR)
        sys.exit(1)

    note_map = {s: fp for s, (_, _, _, fp) in build_note_map().items()}
    updated = unchanged = errors = 0

    for note_path in sorted(VAULT_DIR.glob("*.md")):
        stem = note_path.stem
        if stem not in note_map:
            logger.debug("SKIP  %s", note_path.name)
            unchanged += 1
            continue

        try:
            code_block = extract_code_block(note_map[stem].read_text(errors="replace"))
        except OSError as e:
            logger.error("ERROR reading %s: %s", note_map[stem].name, e)
            errors += 1
            continue

        if DRY_RUN:
            logger.info("DRY-REWRITE  %s", note_path.name)
            updated += 1
            continue

        try:
            if rewrite_code_in_note(note_path, code_block):
                logger.info("REWRITE  %s", note_path.name)
                updated += 1
            else:
                logger.debug("SAME     %s", note_path.name)
                unchanged += 1
        except OSError as e:
            logger.error("ERROR writing %s: %s", note_path.name, e)
            errors += 1

    return updated, unchanged, errors


# ---------------------------------------------------------------------------
# Retag mode
# ---------------------------------------------------------------------------
def retag_all(cf_db: dict, dates: dict) -> tuple[int, int, int]:
    if not VAULT_DIR.exists():
        logger.error("Vault directory not found: %s", VAULT_DIR)
        sys.exit(1)

    note_map = build_note_map()
    # Read submission-id from each note's frontmatter to look up date
    _SUBID_RE = re.compile(r'^submission-id:\s*"?(\d+)"?', re.MULTILINE)
    updated = unchanged = errors = 0

    for note_path in sorted(VAULT_DIR.glob("*.md")):
        stem = note_path.stem
        if stem not in note_map:
            logger.debug("SKIP  %s", note_path.name)
            unchanged += 1
            continue

        sub_id_str, contest_id, prob_letter, fpath = note_map[stem]  # type: ignore[misc]
        cf_tags, diff = lookup_cf(contest_id, prob_letter, cf_db)
        if not cf_tags:
            try:
                cf_tags = infer_tags_fallback(extract_solve(fpath.read_text(errors="replace")))
            except OSError as e:
                logger.error("ERROR reading %s: %s", fpath.name, e)
                errors += 1
                continue
        if not cf_tags:
            cf_tags = ["greedy"]

        sub_date = dates.get(sub_id_str)

        if DRY_RUN:
            logger.info("DRY-RETAG  %s  tags=%s  diff=%s  date=%s",
                        note_path.name, cf_tags, diff, sub_date or "kept")
            updated += 1
            continue

        try:
            if retag_note(note_path, cf_tags, diff, sub_date):
                logger.info("RETAG  %s  date=%s", note_path.name, sub_date or "kept")
                updated += 1
            else:
                logger.debug("SAME   %s", note_path.name)
                unchanged += 1
        except OSError as e:
            logger.error("ERROR writing %s: %s", note_path.name, e)
            errors += 1

    return updated, unchanged, errors


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    logger.info("=" * 60)
    logger.info("sync_to_obsidian  dry=%s  verbose=%s  rewrite=%s  retag=%s",
                DRY_RUN, VERBOSE, REWRITE_CODE, RETAG)
    logger.info("solutions : %s", SOLUTIONS_DIR)
    logger.info("vault     : %s", VAULT_DIR)

    cf_db = fetch_cf_problems(force=REFRESH_CACHE)
    logger.info("CF DB loaded: %d problems", len(cf_db))
    dates = fetch_submission_dates(force=REFRESH_CACHE)
    logger.info("Submission dates loaded: %d accepted submissions", len(dates))

    if REWRITE_CODE:
        updated, unchanged, errors = rewrite_all_code()
        logger.info("-" * 60)
        logger.info("rewrite: updated=%d  unchanged=%d  errors=%d", updated, unchanged, errors)
    elif RETAG:
        updated, unchanged, errors = retag_all(cf_db, dates)
        logger.info("-" * 60)
        logger.info("retag: updated=%d  unchanged=%d  errors=%d", updated, unchanged, errors)
    else:
        created, skipped, errors = sync(cf_db, dates)
        logger.info("-" * 60)
        logger.info("created=%d  skipped=%d  errors=%d", created, skipped, errors)

    logger.info("=" * 60)
    sys.exit(1 if errors else 0)
