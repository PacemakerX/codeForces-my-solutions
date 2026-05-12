#!/usr/bin/env python3
"""
Sync CF solutions → Obsidian vault (one-way, additive only).

Scans all .cpp files in solutions_codForces/, creates a problem note in the
vault for any problem that doesn't have one yet. Never overwrites existing
notes (user annotations are preserved).

The Code block in each note contains everything after the //---...---//
boilerplate separator (helper structs + solve() + main()), not just solve().
The shared template boilerplate is identical across all files and is omitted.

Usage:
    python3 scripts/sync_to_obsidian.py                # normal sync
    python3 scripts/sync_to_obsidian.py --dry-run       # show what would be created
    python3 scripts/sync_to_obsidian.py --verbose       # show skipped files too
    python3 scripts/sync_to_obsidian.py --rewrite-code  # update Code block in all existing notes
"""
import re
import sys
import logging
from datetime import date
from pathlib import Path

# ---------------------------------------------------------------------------
# Paths  (absolute — safe when called from git hook or any cwd)
# ---------------------------------------------------------------------------
REPO_ROOT     = Path(__file__).resolve().parent.parent
SOLUTIONS_DIR = REPO_ROOT / "solutions_codForces"
VAULT_DIR     = Path("/home/sparsh/Documents/vault/01-problems")
LOG_FILE      = REPO_ROOT / "logs" / "sync.log"

DRY_RUN      = "--dry-run"      in sys.argv
VERBOSE      = "--verbose"      in sys.argv
REWRITE_CODE = "--rewrite-code" in sys.argv

# ---------------------------------------------------------------------------
# Logging — always appends to sync.log, also prints to stdout
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
# Tag inference — operates on the solve() body only
# ---------------------------------------------------------------------------
_TAG_RULES = [
    ("Graphs",          [r'\bqueue<', r'\bbfs\b', r'\bdfs\b', r'adj\[',
                         r'adj\.push', r'adj\.emplace', r'visited\[', r'dist\[']),
    ("DP",              [r'\bdp\[', r'\bmemo\[', r'\bndp\[', r'\bf\[.*\]\[', r'\bdp2\[']),
    ("BinarySearch",    [r'while\s*\(\s*lo\s*<\s*hi\s*\)',
                         r'while\s*\(\s*low\s*<=?\s*high\s*\)',
                         r'lo\s*\+\s*\(hi', r'low\s*\+\s*\(high',
                         r'lo\s*\+\s*\(\(hi',
                         r'\blower_bound\s*\(', r'\bupper_bound\s*\(']),
    ("DataStructures",  [r'\bDSU\b', r'\bBIT\b', r'\bfenwick\b', r'\bSegTree\b',
                         r'\bseg\[', r'\bstack<', r'\bordered_set\b', r'\bdeque<']),
    ("BitManipulation", [r'__builtin_popcount', r'\(1\s*<<', r'\(1LL\s*<<',
                         r'__builtin_ctz', r'__builtin_clz', r'\bmask\b', r'\bxor\b']),
    ("Math",            [r'\bgcd\(', r'\blcm\(', r'\bbinpow\(', r'\bmod_pow\(',
                         r'\bmod_inv\(', r'\bsieve\b', r'\bprime\b',
                         r'\bfact\[', r'\binv_fact\[']),
    ("StringAlgo",      [r'\blps\[', r'z_func', r'z\[0\]\s*=\s*n', r'kmp\b',
                         r'\.find\s*\(', r'\.substr\s*\(']),
    ("TwoPointers",     [r'\bl\s*=\s*0.*\br\s*=', r'l\+\+.*r--', r'r--.*l\+\+']),
    ("SlidingWindow",   [r'window', r'for.*r\s*<\s*n.*l\+\+']),
    ("Greedy",          [r'\bsort\s*\(']),
]

def infer_tags(solve_body: str) -> list[str]:
    tags = [tag for tag, pats in _TAG_RULES
            if any(re.search(p, solve_body) for p in pats)]
    return tags if tags else ["Greedy"]


# ---------------------------------------------------------------------------
# Extract code sections from a .cpp file
# ---------------------------------------------------------------------------
_BOILERPLATE_END = re.compile(r'//[-/]+//\s*\n', re.MULTILINE)

def extract_code_block(content: str) -> str:
    """Full file content — the entire .cpp is the code block."""
    return content.strip()

def extract_solve(content: str) -> str:
    """Just the void solve() body — used for tag inference only."""
    after = extract_code_block(content)
    m2 = re.search(r'(void solve\s*\(\s*\).*?)(?=\nint main\s*\()', after, re.DOTALL)
    if m2:
        return m2.group(1).strip()
    idx = after.find('\nint main')
    return after[:idx].strip() if idx != -1 else after.strip()


# ---------------------------------------------------------------------------
# Difficulty heuristic from problem letter
# ---------------------------------------------------------------------------
def diff_from_letter(letter: str) -> str:
    base = letter.upper().rstrip('12')
    return {'A': '~800', 'B': '~1100', 'C': '~1400',
            'D': '~1700', 'E': '~2000'}.get(base, '~2200')


# ---------------------------------------------------------------------------
# Build the markdown note content
# ---------------------------------------------------------------------------
def build_note(sub_id: str, contest_id: str, prob_letter: str,
               code_block: str, tags: list[str]) -> str:
    prob_id   = f"{contest_id}{prob_letter}"
    cf_url    = f"https://codeforces.com/contest/{contest_id}/problem/{prob_letter}"
    diff      = diff_from_letter(prob_letter)
    tags_yaml = ", ".join(tags)
    related   = "\n".join(f"- [[02-patterns/{t}]]" for t in tags)
    today     = date.today().isoformat()

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
# Similarity check — are two solve() bodies the same problem?
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
    all_tokens = set(re.findall(r'[A-Za-z_]\w{2,}', body))
    return all_tokens - _TEMPLATE_TOKENS

def _same_problem(body_a: str, body_b: str) -> bool:
    lines_a = body_a.count('\n') + 1
    lines_b = body_b.count('\n') + 1
    if lines_a and lines_b and max(lines_a, lines_b) / min(lines_a, lines_b) > 2.0:
        return False
    sig_a = _signature_tokens(body_a)
    sig_b = _signature_tokens(body_b)
    if not sig_a and not sig_b:
        return True
    if not sig_a or not sig_b:
        return False
    overlap = len(sig_a & sig_b) / min(len(sig_a), len(sig_b))
    return overlap >= 0.4


# ---------------------------------------------------------------------------
# Collect .cpp files → list of (note_stem, sub_id, contest_id, prob_letter, path)
# ---------------------------------------------------------------------------
def collect_problems() -> list[tuple]:
    grouped: dict[str, list[tuple]] = {}
    for f in SOLUTIONS_DIR.glob("*.cpp"):
        m = re.match(r'^(\d+)_(\d+)_([A-Za-z0-9]+)\.cpp$', f.name)
        if not m:
            continue
        sub_id, contest_id, prob_letter = m.groups()
        prob_letter = prob_letter.upper()
        key = f"{contest_id}_{prob_letter}"
        grouped.setdefault(key, []).append((int(sub_id), contest_id, prob_letter, f))

    entries: list[tuple] = []
    for key, subs in grouped.items():
        subs.sort(key=lambda x: x[0], reverse=True)
        contest_id  = subs[0][1]
        prob_letter = subs[0][2]

        if len(subs) == 1:
            sub_id, _, _, fpath = subs[0]
            entries.append((f"CF_{contest_id}_{prob_letter}",
                            str(sub_id), contest_id, prob_letter, fpath))
            continue

        newest = subs[0]
        oldest = subs[-1]

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


# ---------------------------------------------------------------------------
# Rewrite the ## Code block in an existing note without touching anything else
# ---------------------------------------------------------------------------
_CODE_BLOCK_RE = re.compile(
    r'(## Code\n```cpp\n).*?(```)',
    re.DOTALL
)

def rewrite_code_in_note(note_path: Path, new_code: str) -> bool:
    """Replace the cpp code block content. Returns True if file was changed."""
    text = note_path.read_text(encoding="utf-8")
    replacement = rf'\g<1>{new_code}\n\g<2>'
    new_text, n = _CODE_BLOCK_RE.subn(replacement, text, count=1)
    if n == 0:
        return False  # pattern not found — skip
    if new_text == text:
        return False  # no change
    note_path.write_text(new_text, encoding="utf-8")
    return True


# ---------------------------------------------------------------------------
# Build a mapping: note_stem → cpp path (newest submission wins)
# ---------------------------------------------------------------------------
def build_note_to_cpp_map() -> dict[str, Path]:
    entries = collect_problems()
    return {stem: fpath for stem, _, _, _, fpath in entries}


# ---------------------------------------------------------------------------
# Main sync — create missing notes
# ---------------------------------------------------------------------------
def sync() -> tuple[int, int, int]:
    """Returns (created, skipped, errors)"""
    if not VAULT_DIR.exists():
        logger.error("Vault directory not found: %s", VAULT_DIR)
        sys.exit(1)

    entries = collect_problems()
    logger.info("Found %d entries to sync (from solutions_codForces/)", len(entries))

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

        solve_body = extract_solve(content)
        code_block = extract_code_block(content)
        tags       = infer_tags(solve_body)
        note       = build_note(sub_id, contest_id, prob_letter, code_block, tags)

        if DRY_RUN:
            logger.info("DRY   %s  tags=%s", note_name, tags)
        else:
            note_path.write_text(note, encoding="utf-8")
            logger.info("NEW   %s  tags=%s", note_name, tags)

        created += 1

    return created, skipped, errors


# ---------------------------------------------------------------------------
# Rewrite mode — update Code block in every existing note
# ---------------------------------------------------------------------------
def rewrite_all_code() -> tuple[int, int, int]:
    """Returns (updated, unchanged, errors)"""
    if not VAULT_DIR.exists():
        logger.error("Vault directory not found: %s", VAULT_DIR)
        sys.exit(1)

    note_map = build_note_to_cpp_map()
    updated = unchanged = errors = 0

    for note_path in sorted(VAULT_DIR.glob("*.md")):
        stem = note_path.stem
        if stem not in note_map:
            logger.debug("SKIP  %s (no matching .cpp)", note_path.name)
            unchanged += 1
            continue

        fpath = note_map[stem]
        try:
            content   = fpath.read_text(errors="replace")
            code_block = extract_code_block(content)
        except OSError as e:
            logger.error("ERROR reading %s: %s", fpath.name, e)
            errors += 1
            continue

        if DRY_RUN:
            logger.info("DRY-REWRITE  %s", note_path.name)
            updated += 1
            continue

        try:
            changed = rewrite_code_in_note(note_path, code_block)
        except OSError as e:
            logger.error("ERROR writing %s: %s", note_path.name, e)
            errors += 1
            continue

        if changed:
            logger.info("REWRITE  %s", note_path.name)
            updated += 1
        else:
            logger.debug("SAME     %s", note_path.name)
            unchanged += 1

    return updated, unchanged, errors


if __name__ == "__main__":
    logger.info("=" * 60)
    logger.info("sync_to_obsidian  dry_run=%s  verbose=%s  rewrite_code=%s",
                DRY_RUN, VERBOSE, REWRITE_CODE)
    logger.info("solutions : %s", SOLUTIONS_DIR)
    logger.info("vault     : %s", VAULT_DIR)

    if REWRITE_CODE:
        updated, unchanged, errors = rewrite_all_code()
        logger.info("-" * 60)
        logger.info("rewrite: updated=%d  unchanged=%d  errors=%d",
                    updated, unchanged, errors)
    else:
        created, skipped, errors = sync()
        logger.info("-" * 60)
        logger.info("created=%d  skipped=%d  errors=%d", created, skipped, errors)

    logger.info("=" * 60)
    sys.exit(1 if errors else 0)
