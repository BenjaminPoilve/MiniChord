#! /usr/bin/env bash
source venv/bin/activate
python3 -m mkdocs build
cp docs/index.html site/
cp -R ../firmware/minicontrol site/