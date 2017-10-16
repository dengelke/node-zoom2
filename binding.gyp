{
  'targets': [
    {
      'target_name': 'zoom',
      "cflags": ["<!@(yaz-config --libs --include)"],
      "libraries": ["<!@(yaz-config --libs)"],
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
