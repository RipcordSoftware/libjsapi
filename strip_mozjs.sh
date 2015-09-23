#!/bin/bash
# strip the debug table from libmozjsi and js
strip -g externals/installed/lib/libjs_static.ajs
strip -g externals/installed/bin/js
