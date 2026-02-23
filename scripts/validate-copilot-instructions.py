#!/usr/bin/env python3
"""Validate .github/copilot-instructions.md structure and referenced source files."""

import os
import re
import sys


def find_repo_root():
    """Find the repository root by walking up from this script's location."""
    path = os.path.dirname(os.path.abspath(__file__))
    while path != os.path.dirname(path):
        if os.path.isdir(os.path.join(path, ".git")):
            return path
        path = os.path.dirname(path)
    # Fallback: assume script is in scripts/ under repo root
    return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def validate_markdown_structure(content):
    """Check basic markdown structure: headings, code blocks balance."""
    errors = []
    lines = content.split("\n")

    # Check for at least one top-level heading
    has_heading = any(line.startswith("# ") for line in lines)
    if not has_heading:
        errors.append("No top-level heading (# ) found")

    # Check balanced code fences
    fence_count = sum(1 for line in lines if line.strip().startswith("```"))
    if fence_count % 2 != 0:
        errors.append(f"Unbalanced code fences: {fence_count} found (expected even)")

    return errors


def find_src_references(content):
    """Extract file paths under src/ referenced in the markdown."""
    # Match patterns like src/path/to/file.ext
    pattern = r'(?:^|\s|`|"|\()((src/[a-zA-Z0-9_/\-]+\.[a-zA-Z0-9_]+))'
    matches = re.findall(pattern, content)
    return sorted(set(m[1] if isinstance(m, tuple) else m for m in matches))


def main():
    repo_root = find_repo_root()
    instructions_path = os.path.join(repo_root, ".github", "copilot-instructions.md")

    if not os.path.isfile(instructions_path):
        print(f"INFO: {instructions_path} not found, skipping validation.")
        return 0

    with open(instructions_path, "r", encoding="utf-8") as f:
        content = f.read()

    print(f"Validating: {instructions_path}")
    print(f"  File size: {len(content)} bytes")

    # Validate markdown structure
    structure_errors = validate_markdown_structure(content)
    for err in structure_errors:
        print(f"  ERROR: {err}")

    if structure_errors:
        print("Markdown structure validation FAILED.")
        return 1

    print("  Markdown structure: OK")

    # Check referenced src/ files
    refs = find_src_references(content)
    if not refs:
        print("  No src/ file references found.")
    else:
        missing = 0
        for ref in refs:
            full_path = os.path.join(repo_root, ref)
            if os.path.exists(full_path):
                print(f"  FOUND: {ref}")
            else:
                print(f"  WARNING: referenced file not found: {ref}")
                missing += 1

        print(f"  Referenced files: {len(refs)} total, {missing} missing")

    print("Validation passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
