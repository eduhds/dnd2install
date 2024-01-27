#!/bin/sh

sudo cp build/release/dnd2install dnd2install-* /usr/bin && \
  sudo cp dnd2install.desktop /usr/share/applications && \
  sudo chmod +x /usr/bin/dnd2install-*
