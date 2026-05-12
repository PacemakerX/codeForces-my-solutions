#!/usr/bin/env bash
# Install git hooks for the codeForces-my-solutions repo.
# Run once from anywhere inside the repo:
#   bash scripts/setup_hooks.sh

set -euo pipefail

REPO_ROOT="$(git -C "$(dirname "$0")" rev-parse --show-toplevel)"
HOOKS_DIR="$REPO_ROOT/.git/hooks"
SCRIPTS_DIR="$REPO_ROOT/scripts"

echo "Repo root : $REPO_ROOT"
echo "Hooks dir : $HOOKS_DIR"
echo ""

# ---------------------------------------------------------------------------
# post-commit
# ---------------------------------------------------------------------------
SRC="$SCRIPTS_DIR/post-commit"
DST="$HOOKS_DIR/post-commit"

if [[ ! -f "$SRC" ]]; then
    echo "ERROR: $SRC not found. Run this script from the repo root." >&2
    exit 1
fi

cp "$SRC" "$DST"
chmod +x "$DST"
echo "✓ post-commit hook installed  →  $DST"

# ---------------------------------------------------------------------------
# Smoke-test: verify Python 3 is available
# ---------------------------------------------------------------------------
if command -v python3 &>/dev/null; then
    PY_VER="$(python3 --version 2>&1)"
    echo "✓ Python found : $PY_VER"
else
    echo "⚠  WARNING: python3 not found in PATH."
    echo "   The hook will skip vault sync until python3 is available."
fi

# ---------------------------------------------------------------------------
# Smoke-test: verify vault path exists
# ---------------------------------------------------------------------------
VAULT_DIR="/home/sparsh/Documents/vault/01-problems"
if [[ -d "$VAULT_DIR" ]]; then
    echo "✓ Vault directory exists : $VAULT_DIR"
else
    echo "⚠  WARNING: vault directory not found at $VAULT_DIR"
    echo "   Edit VAULT_DIR in scripts/sync_to_obsidian.py if the path changed."
fi

echo ""
echo "Setup complete."
echo ""
echo "The hook runs automatically on every 'git commit' that adds new .cpp files."
echo "Logs are written to: $REPO_ROOT/logs/sync.log"
echo ""
echo "Manual sync:"
echo "  python3 scripts/sync_to_obsidian.py             # sync all new"
echo "  python3 scripts/sync_to_obsidian.py --dry-run   # preview only"
echo "  python3 scripts/sync_to_obsidian.py --verbose   # show skipped too"
