sources = files([
    'asynctest.c',
])
libldb_asynctest = shared_library('ldb-asynctest',
        sources,
        dependencies: ldb_asynctask_dep,
        include_directories: include_directories('.'),
        install: true,
        version: meson.project_version(),
        soversion: so_version,
    )