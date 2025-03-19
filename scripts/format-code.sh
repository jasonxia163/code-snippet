#!/bin/bash
project_root=$(cd $(dirname $0)/.. && pwd)

find ${project_root}/cpp -type f \( -name "*.h" -o -name "*.cc" -o -name "*.cpp" \) | xargs clang-format -i -style=file
