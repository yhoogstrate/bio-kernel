
project('bio-kernel', 'c',
    version : '0.0.1',
    default_options : ['warning_level=3', 'c_std=c11'])


src = ['utils.c']





executable('bio',
    [src, 'main_bio.c'],
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)



executable('lsb',
    [src, 'main_lsb.c'],
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)

executable('chtax',
    [src, 'main_chtax.c'], 
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)

executable('rmtax',
    [src, 'main_rmtax.c'], 
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)


kernel_version = run_command('uname', '-r').stdout().strip()

custom_target('biokernel',
	input: ['super.c'],
	command: ['touch', 'bk.ko'],
	#command: ['make', '-C', '/lib/modules/'+kernel_version+ '/build' ,'M=$(PWD)', 'modules'],
	output: 'biokernel.ko'
	)






