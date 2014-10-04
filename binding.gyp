{
  'variables': {
      "module_name":"<!(node -e \"console.log(require('../package.json').binary.module_name)\")",
      "module_path":"<!(node -e \"console.log(require('../package.json').binary.module_path)\")",
  },
    "targets": [{
        "target_name": "nativeMinify",
        "sources": ["src/main.cc"],
        'conditions': [
            ['OS=="linux"', {
                'cflags': [
                    '-std=c++11'
                ]
            }]
        ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
          {
            "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
            "destination": "<(module_path)"
          }
      ]
    }]
}
