#/bin/csh
echo $#
ftp $1 << EOF
binary
prompt
cd E:/code/
lcd /root/code/
EOF