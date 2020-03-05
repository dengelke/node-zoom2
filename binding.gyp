{
  'targets': [
    {
      'target_name': 'zoom',
      "cflags": ["<!@(yaz-config --libs <!@(if test '4' = <!@(pkg-config --modversion yaz|cut -f1 -d '.');then echo -n '--cflags';else echo -n '--include';fi))"],
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
