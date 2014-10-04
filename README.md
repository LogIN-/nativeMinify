## Introduction
`native-minify` is a JS minify tool which removes comments and unnecessary whitespace from JavaScript files. 

It allow you to compress JavaScript typically reducing file-size by half.

Please be careful with your original files this minify-cation is one-way process only.

## What `native-minify` does?

* It omits some characters in JavaScript files.
* It replaces all control characters (including tab) with spaces.
* It replaces comments in the "// ..." and "/\* ... \*/" form with whitespace.
* All spaces and newlines when acceptable are replaced with a single space
* It is careful when omitting newlines, because their are sometimes treated as semicolons. 

## What `native-minify` doesn't do?
* It doesn't modify any other characters in your code.
* It doesn't obfuscate your code.
* If your code looks like this it wont work just yet:

```javascript
if ( _lineCount === _linePoolLength ) {

    var line = new THREE.RenderableLine();
    _linePool.push( line );
    _linePoolLength ++;
    _linePoolLength++;
    _lineCount ++
    return line;

}
```

* If your code in worst case looks like this it will work:

```javascript
var crazyCode = {
    test01: 1,
    test02: 2,
    test03: 3
}
crazyCode.test04 = 4;
```
## How to use
```javascript
var JSMin = require('native-minify');
JSMin.minifyFile([  '/full_path_to_file_input/backbone.js',
                    '/full_path_to_file_input/three.js'], 
                    '/full_path_to_file_output/compacted.min.js', 
                    function(results){
                        console.log(results);
                    }
);
```
or
```javascript
var JSMin = require('native-minify');
JSMin.minifyFile(['/full_path_to_file_input/backbone.js'], '/full_path_to_file_output/compacted.min.js');
```
## Character Set
`native-minify` requires that the character set encoding of the input is ASCII or UTF-8.

## Simple Benchmark
This is simple benchmark on minifying 100 underscore.js files

| `native-minify` | `uglifyjs`      |
|:---------------:|:---------------:|
| 400ms           | 5007ms          |


## Support and Bugs
If you are having trouble, have found a bug, or want to contribute don't be shy.

I did a very little testing so if your JS code is valid but after minimization doesn't work please open a bug ticket so we can fix that!

[Open a ticket](https://github.com/LogIN-/nativeMinify/issues) on GitHub.

## License
`native-minify` source-code uses the The MIT License (MIT), see our `LICENSE` file.
```
The MIT License (MIT)
Copyright (c) LogIN- 2014
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```