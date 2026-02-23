"""Smoke tests for the HexMag inference engine."""

import sys
import requests

BASE_URL = "http://localhost:8001"


def test_health():
    resp = requests.get(f"{BASE_URL}/health", timeout=5)
    assert resp.status_code == 200
    data = resp.json()
    assert data["status"] == "ok"
    assert data["engine"] == "hexmag"
    print("PASS: /health")


def test_ask():
    resp = requests.post(
        f"{BASE_URL}/ask",
        json={"prompt": "What is cloud hosting?"},
        timeout=5,
    )
    assert resp.status_code == 200
    data = resp.json()
    assert "response" in data
    assert data["model"] == "hexmag-v1"
    assert data["tokens_used"] > 0
    print("PASS: /ask")


def test_agent():
    resp = requests.post(
        f"{BASE_URL}/agent",
        json={"task": "deploy service"},
        timeout=5,
    )
    assert resp.status_code == 200
    data = resp.json()
    assert isinstance(data["plan"], list)
    assert len(data["plan"]) > 0
    assert data["status"] == "complete"
    print("PASS: /agent")


def main():
    tests = [test_health, test_ask, test_agent]
    failed = 0
    for test in tests:
        try:
            test()
        except Exception as e:
            print(f"FAIL: {test.__name__} - {e}")
            failed += 1
    if failed:
        print(f"\n{failed}/{len(tests)} tests failed")
        sys.exit(1)
    print(f"\nAll {len(tests)} tests passed")
    sys.exit(0)


if __name__ == "__main__":
    main()
