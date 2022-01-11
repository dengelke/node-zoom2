{
  'targets': [
    {
      'target_name': 'binding',
      'conditions': [
        ['OS!="win"', {
          'dependencies': [
            'binding-not-win.gyp:zoom',
          ],
        }],
        ['OS=="win"', {
          'dependencies': [
            'binding-win.gyp:zoom',
          ],
        }],
      ],
    },
  ],
}