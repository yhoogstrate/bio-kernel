
project('bio-kernel', 'c',
    version : '0.0.1',
    default_options : ['warning_level=3', 'c_std=c11'])



lib_src = ['src/lib/utils.c', 'src/lib/tax.c']





executable('bio',
    [lib_src, 'src/bin/main_bio.c'],
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)



executable('lsb',
    [lib_src, 'src/bin/main_lsb.c'],
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)

executable('chtax',
    [lib_src, 'src/bin/main_chtax.c'], 
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)

executable('rmtax',
    [lib_src, 'src/bin/main_rmtax.c'], 
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)

executable('chref',
    [lib_src, 'src/bin/main_chref.c'], 
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)

executable('service-index-taxons',
    [lib_src, 'src/bin/service_index_taxons.c'],
    include_directories : include_directories('.'),
    dependencies: [],
    install: true)




kernel_version = run_command('uname', '-r',
    check: true
    ).stdout().strip()


#custom_target('biokernel',
#	input: ['super.c'],
#	command: ['touch', 'bk.ko'],
#	#command: ['make', '-C', '/lib/modules/'+kernel_version+ '/build' ,'M=$(PWD)', 'modules'],
#	output: 'biokernel.ko'
#	)



conf_data = configuration_data()
conf_data.set('BIOKERNEL_VERSION', meson.project_version())
conf_data.set('COMPILE_DATE', run_command('bash', '-c', 'date -u' ,   check: true).stdout().strip())



configure_file(
  input : 'src/config.h.in',
  output : 'config.h',
  configuration : conf_data
)



