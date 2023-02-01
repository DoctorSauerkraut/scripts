#!/bin/sh

# Extract generical information from a file

file=$1

md5=$(md5sum $file | cut -d ' ' -f 1)
sha1=$(sha1sum $file | cut -d ' ' -f 1)
sha256=$(sha256sum $file | cut -d ' ' -f 1)
data=$(file $file | cut -d ':' -f 2)
stat=$(stat $file)

# Requires vt-cli : https://github.com/VirusTotal/vt-cli
vtres=$(vt file $md5 --include last_analysis_stats 2> /dev/null)


echo "--- FILE INFO : $file ---"
echo "MD5:    $md5"
echo "SHA1:   $sha1"
echo "SHA256: $sha256"
echo "Type:  $data"
if test -z "$vtres" 
then
    echo "VTScore:Unknown"
else
    echo "VTScore:$vtres"
fi
echo "Stat: $stat"
echo "--- END INFO ---"
