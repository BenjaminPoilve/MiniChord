#! /usr/bin/env bash
mkdocs build
cp docs/index.html site/
cp -R ../firmware/mini_control site/