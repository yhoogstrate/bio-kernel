#!/usr/bin/env python



from pywget import wget
import tarfile
from tqdm import tqdm
import os

addr = "ftp://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz.md5"
wget.download(addr, ".")
print("\n")
with open('taxdump.tar.gz.md5', "r") as fh:
    md5 = fh.read().strip().split()[0]
    os.remove('taxdump.tar.gz.md5')



cur_md5 = "  [ !!! no taxons file present !!! ]  "
if os.path.isfile("taxons"):
    with open("taxons", "r") as fh:
        for line in fh:
            if line[0:21] == "# MD5 taxdump.tar.gz:":
                cur_md5 = line[21:].strip()
                break


if md5 != cur_md5:
    print("updating to latest taxons: "+cur_md5+" -> " + md5)

    addr = "ftp://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz"
    wget.download(addr, ".")


    with open("taxons.tmp", "w") as fh_w:
        with tarfile.open("taxdump.tar.gz","r:gz") as tar:
            n = 0
            for tarinfo in tar:
                if(tarinfo.name == "names.dmp"):
                    f = tar.extractfile(tarinfo)
                    for line in tqdm(f):
                        line = [_.strip() for _ in line.decode('ascii').strip().split('|')]
                        
                        if line[3] == 'scientific name':
                            fh_w.write(line[0] + "\t" + line[1] + "\n")
                            n += 1

    os.remove('taxdump.tar.gz')
    
    with open("taxons.tmp", "r") as fh_r:
        with open("taxons", "w") as fh_w:
            fh_w.write("# MD5 taxdump.tar.gz: " + md5 + "\n")
            fh_w.write("# n: "+str(n)+"\n")
            
            for line in tqdm(fh_r):
                fh_w.write(line)
    
    os.remove("taxons.tmp")

else:
    print("taxons is already up to date: " + cur_md5)

