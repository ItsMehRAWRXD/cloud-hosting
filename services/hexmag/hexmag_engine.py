"""HexMag inference engine - Flask-based model serving endpoint."""

import argparse
from flask import Flask, request, jsonify

app = Flask(__name__)


@app.route("/health", methods=["GET"])
def health():
    return jsonify({"status": "ok", "engine": "hexmag", "version": "1.0.0"})


@app.route("/ask", methods=["POST"])
def ask():
    data = request.get_json(force=True)
    prompt = data.get("prompt", "")
    tokens_used = len(prompt.split())
    return jsonify({
        "response": f"HexMag response to: {prompt}",
        "model": "hexmag-v1",
        "tokens_used": tokens_used,
    })


@app.route("/agent", methods=["POST"])
def agent():
    data = request.get_json(force=True)
    task = data.get("task", "")
    plan = [
        {"step": 1, "action": "analyze", "detail": f"Analyze task: {task}"},
        {"step": 2, "action": "execute", "detail": "Execute plan"},
        {"step": 3, "action": "verify", "detail": "Verify results"},
    ]
    return jsonify({"plan": plan, "status": "complete"})


@app.route("/v1/models", methods=["GET"])
def list_models():
    return jsonify({"data": [{"id": "hexmag-v1", "object": "model"}]})


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="HexMag Inference Engine")
    parser.add_argument("--port", type=int, default=8000, help="Port to listen on")
    args = parser.parse_args()
    app.run(host="0.0.0.0", port=args.port)
