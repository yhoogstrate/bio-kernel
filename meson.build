
project('bio-kernel', 'c',
    version : '0.0.1',
    default_options : ['warning_level=3', 'c_std=c11'])


src = ['utils.c']



executable('bio', [src, 'main_bio.c'], 
    include_directories : include_directories('.'),
    dependencies: [])

executable('bio-kerneld', [src, 'main_bio-kerneld.c'], 
    include_directories : include_directories('.'),
    dependencies: [])



executable('lsb', [src, 'main_lsb.c'], 
    include_directories : include_directories('.'),
    dependencies: [])

executable('chtax', [src, 'main_chtax.c'], 
    include_directories : include_directories('.'),
    dependencies: [])

executable('rmtax', [src, 'main_rmtax.c'], 
    include_directories : include_directories('.'),
    dependencies: [])

