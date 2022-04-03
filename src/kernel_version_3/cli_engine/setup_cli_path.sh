echo "Setting Python Paths"
python << EOF
import sys
sys.path.append("/aisa/current/src/user/cli/")
sys.path.append("/aisa/current/src/user/cli/mdraid")
sys.path.append("/aisa/current/src/kernel/dmtarget")
EOF

