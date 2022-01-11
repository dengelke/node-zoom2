{
  'targets': [
    {
      'target_name': 'zoom',
      'libraries': ["C:\/Program Files\/YAZ\/lib\/yaz5.lib"],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        'C:/Program Files/YAZ/include',
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'ExceptionHandling': 1,
        },
        "VCLinkerTool": {
          "LinkTimeCodeGeneration": 1,
          "OptimizeReferences": 2,
          "EnableCOMDATFolding": 2,
          "LinkIncremental": 1,
        }        
      },
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
