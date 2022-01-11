{
  'targets': [
    {
      'target_name': 'binding',
      'conditions': [
        ['OS=="mac"', {
          'dependencies': [
            'binding-mac.gyp:zoom',
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