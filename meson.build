project('ldb-asynctest', ['c'],
    license: 'LGPL-2.1+',
    version: '0.0.1',
    meson_version: '>=0.53',
    default_options: ['warning_level=2', 'c_std=c17', 'werror=true'])

ldb_asynctask_dep = dependency('ldb-asynctask',
    fallback : ['ldb-asynctask', 'ldb_asynctask_dep'])