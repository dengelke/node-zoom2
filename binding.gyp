{
  'targets': [
    {
      'target_name': 'zoom',
      'cflags': [],
      'libraries': [],
      'dependencies': [
        '<(module_root_dir)/deps/yaz/yaz.gyp:yaz'
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'sources': [
        'src/zoom.cc',
        'src/query.cc',
        'src/record.cc',
        'src/errors.cc',
        'src/records.cc',
        'src/options.cc',
        'src/resultset.cc',
        'src/connection.cc'
      ]
    }
  ]
}
