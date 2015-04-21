#!/bin/bash
# strip the debug table from libmozjs
strip -g externals/installed/lib/libmozjs-??.a
