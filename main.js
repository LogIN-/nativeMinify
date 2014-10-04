/* 
 * @Author: login
 * @Date:   2014-09-30 09:17:23
 * @Last Modified by:   login
 * @Last Modified time: 2014-10-03 11:34:51
 */
var fs = require('fs');
var path = require('path');
var compressor = require('node-minify');
var JSMin = require('./src/build/Release/nativeMinify');


var mesureTime = false;

if(mesureTime){
    console.time("native-minify");
    for (var i = 0; i <= 100; i++) {
        JSMin.minifyFile('/home/login/documents/repos/nativeMinify/src/testFiles/in/backbone.js', '/home/login/documents/repos/nativeMinify/src/testFiles/out/native/backbone' +  i + '.js');
    };
    console.timeEnd("native-minify")    
}


if(mesureTime){
    console.time("javascript-minify");
    for (var i = 0; i <= 100; i++) {
        // Using Google Closure
        new compressor.minify({
            type: 'uglifyjs',
            fileIn: '/home/login/documents/repos/nativeMinify/src/testFiles/in/backbone.js',
            fileOut: '/home/login/documents/repos/nativeMinify/src/testFiles/out/others/backbone' +  i + '.js',
            callback: function(err, min){
                if(err){
                    console.log(err);    
                }
            }
        });
    };
    console.timeEnd("javascript-minify")
}

if(mesureTime == false){
    console.log("Callback function:")
    JSMin.minifyFile([
                        '/home/login/documents/repos/nativeMinify/src/testFiles/in/backbone.js',
                        '/home/login/documents/repos/nativeMinify/src/testFiles/in/three.js'], 
                        '/home/login/documents/repos/nativeMinify/src/testFiles/out/native/compacted.min.js', 
                        function(results){
                            console.log(results);
                        }
                    );
    // console.log("Classic function:")
    // JSMin.minifyFile([
    //                     '/home/login/documents/repos/nativeMinify/src/testFiles/in/backbone.js'], 
    //                     '/home/login/documents/repos/nativeMinify/src/testFiles/out/native/backbone.min.js'
    //                 );
}

// fs.readFile(JSCode, 'utf8', function(err, data) {
//     if (err) {
//         return console.log(err);
//     }
//     var test = JSMin.minifyFile(data, "./src/testFiles/backbone.min.js");
// 
//     console.log("***********************");
//     console.log(test);
//     //JSMinify.minifyString("sd");
// });
