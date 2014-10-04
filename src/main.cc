#include <node.h>
#include <v8.h>

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>

#include <iostream>
#include <fstream>

#include <string.h>
#include <ctype.h>

using namespace v8;
using namespace std;   

std::string currentLine;

char previousLetter;
char currentLetter;
char nextLetter;

char lastWrittenLetter;
char commentControll;
char whiteSpaceControll;

std::ofstream outFileStream;
const char* inputFile;    
const char* outputFile;

unsigned int currentLetterPos;
size_t lineLength;

unsigned int totalLetters;
unsigned int finalLetters;

std::vector<std::string> reservedJSWords{"abstract", "alert", "Anchor", "anchors", "Applet", "applets", "Area", "Array", "as", "back", "blur", "Body", "boolean", "break", "Button", "byte", "captureevents", "case", "catch", "char", "Checkbox", "class", "clearinterval", "cleartimeout", "close", "closed", "Components", "confirm", "const", "content", "continue", "controllers", "crypto", "Date", "debugger", "decodeURI", "decodeURIComponent", "default", "defaultstatus", "delete", "directories", "do", "document", "double", "dump", "else", "encodeURI", "encodeURIComponent", "enum", "Error", "escape", "eval", "EvalError", "export", "extends", "false", "FileUpload", "final", "finally", "float", "focus", "for", "Form", "forms", "forward", "frame", "frames", "function", "getAttention", "getSelection", "goto", "Hidden", "history", "home", "if", "Image", "images", "implements", "import", "in", "Infinity", "innerHeight", "innerWidth", "instanceof", "int", "interface", "is", "isFinite", "isNaN", "length", "let", "Link", "links", "location", "locationbar", "long", "Math", "menubar", "MimeType", "mimetypes", "moveBy", "moveTo", "name", "namespace", "NaN", "native", "navigator", "new", "null", "Number", "Object", "onabort", "onblur", "onchange", "onclick", "onclose", "ondragdrop", "onerror", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onload", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup", "onpaint", "onreset", "onresize", "onscroll", "onselect", "onsubmit", "onunload", "open", "opener", "Option", "options", "outerHeight", "outerWidth", "package", "pageXOffset", "pageYOffset", "parent", "parsefloat", "parseint", "Password", "personalbar", "pkcs", "Plugin", "plugins", "print", "private", "prompt", "prompter", "protected", "public", "Radio", "RangeError", "ReferenceError", "RegExp", "releaseevents", "Reset", "resizeBy", "resizeTo", "return", "screen", "screenX", "screenY", "Script", "scroll", "scrollbars", "scrollBy", "scrollByLines", "scrollByPages", "scrollTo", "scrollX", "scrollY", "Select", "self", "setCursor", "setinterval", "settimeout", "short", "sizeToContents", "static", "statusbar", "stop", "String", "Style", "StyleSheet", "Submit", "super", "switch", "synchronized", "SyntaxError", "Text", "Textarea", "this", "throw", "throws", "toolbar", "top", "transient", "true", "try", "TypeError", "typeof", "undefined", "unescape", "updateCommands", "URIError", "use", "var", "void", "volatile", "while", "window", "with", "yield"};

/* isAlphanum -- return true if the character is a letter, digit, underscore,
        dollar sign, or non-ASCII character.
*/

static int isAlphanum(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') || c == '_' || c == '+' || c == '$' || c == '\\' ||
        c > 126);
}
/* ToCString
 * Extracts a C string from a V8 Utf8Value. 
 * return C String
 */
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}
/* exsistOutFileStream
 * checks if outpuFile exist on file-system
 */
inline bool exsistOutFileStream () {
  struct stat buffer;   
  return (stat (outputFile, &buffer) == 0); 
}
/* removeOutFileStream
 * removes (delete) output file on file-system
 */
static void removeOutFileStream() {  
    int ret_code = std::remove(outputFile);
    if (ret_code != 0) {
        std::cerr << "Error during the deletion: " << ret_code << '\n';
    }
}
/* openOutFileStream
 * opens output file for writing
 * if file doesnt exist it creates it
 */
static void openOutFileStream() {  
    outFileStream.open(outputFile);
    if(!outFileStream){
        std::cout << "Opening file failed" << endl;
    }

}
/* closeOutFileStream
 * closes output stream file
 * this is usually done automatically and
 * its unnecessarily
 */
static void closeOutFileStream() {  
    outFileStream.close();       //close file
    //assert(!outFileStream.fail());
}
/* appendOutFileStream
 * writes data to output file stream
 */
static void appendOutFileStream(char data) {
    // use operator<< for clarity
    outFileStream << data;
    // test if write was succesful - not *really* necessary
    if(!outFileStream){
        std::cout << "Write failed" << endl;
    }

}
/* SkipBOMMarking
 * Skips the Byte Order Mark (BOM) that defines UTF-8 in some text files.
 */
void SkipBOMMarking(std::ifstream &in){
    char test[3] = {0};
    in.read(test, 3);
    if ((unsigned char)test[0] == 0xEF && 
        (unsigned char)test[1] == 0xBB && 
        (unsigned char)test[2] == 0xBF)
    {
        return;
    }
    in.seekg(0);
}
static void jsmin(){
    /* Populate letters* variables */
    if(currentLetterPos > 1){
        previousLetter = currentLine.at(currentLetterPos - 1);
    }else{
        previousLetter = ' ';
    }    
    if(currentLetterPos < lineLength - 1){
        nextLetter = currentLine.at(currentLetterPos + 1);
    }else{
        nextLetter = ' ';
    }    
    currentLetter = currentLine.at(currentLetterPos);

    /* Check for comment start */
    if(commentControll == 0){
        if(currentLetter == '/' && nextLetter == '/'){
            commentControll = 1;
        }else if (currentLetter == '/' && nextLetter == '*'){
            commentControll = 2;
        }
    }
    /* If we are not in comment */
    if(commentControll == 0){
        /* If we have two whitespace we will skip until only one */
        if(currentLetter == ' ' && nextLetter == ' '){
            whiteSpaceControll = 1;
        }else{
            whiteSpaceControll = 0;
        }
        /* only one whitespace is left */
        if(currentLetter != '\n' && currentLetter != '\r' && currentLetter != '\t' && whiteSpaceControll == 0){
            if( (isAlphanum(nextLetter) == false || isAlphanum(previousLetter) == false) && currentLetter == ' '){
                whiteSpaceControll = 1;
            }else{
                whiteSpaceControll = 0;
            }
            if(whiteSpaceControll == 0){
                if(lastWrittenLetter != 0 && lastWrittenLetter == '}' && isalnum(currentLetter)){
                    appendOutFileStream('\n');
                }
                lastWrittenLetter = currentLetter;
                appendOutFileStream(currentLetter);
                ++finalLetters;
            }
        }
        
    }

    /* Check if comment is ended */
    if(commentControll == 1 && currentLetter == '\n'){
        commentControll = 0;
    }
    if(commentControll == 2 && currentLetter == '/' && previousLetter == '*'){
        commentControll = 0;
    }

}
vector<string> explode(const string& str, const char& ch) {
    string next;
    vector<string> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
}
/* main -- Output any command line arguments as comments
        and then minify the input.
*/
Handle<Value> minifyFile(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 2 || args.Length() > 3) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    /* second argument is output file */
    v8::String::Utf8Value outputFileArg(args[1]);
    outputFile = ToCString(outputFileArg);
    if(exsistOutFileStream()){
        removeOutFileStream();
    }
    openOutFileStream();

    /* first argument is input file or array of files */
    v8::String::Utf8Value inputFileArg(args[0]);
    inputFile = ToCString(inputFileArg);  

    /* read and process input files one by one to output */
    std::vector<std::string> inputFileArray = explode(inputFile, ',');

    Local<Object> resOutput = Object::New();
    // Create a new empty array.
    Handle<Array> resOutputItemArray = Array::New(inputFileArray.size());
    

    for (size_t i = 0; i < inputFileArray.size(); i++) {

        Local<Object> resOutputItemObject = Object::New();

        /* Set current input file to input stream */
        std::ifstream input(inputFileArray[i]);
        /* Skip custom BOM markings */
        SkipBOMMarking(input);         

        /* reset variables */
        lastWrittenLetter = 0;
        commentControll = 0;
        whiteSpaceControll = 0;
        totalLetters = 0;
        finalLetters = 0;

        /* Read file input line by line */
        for(currentLine; getline(input, currentLine);){   
            currentLine += '\n';
            lineLength = currentLine.length();
            /* Split line into chars */
            for (currentLetterPos = 0; currentLetterPos < lineLength; ++currentLetterPos)
            {
                jsmin();
                ++totalLetters;
            }
        }

        /* Debug output */
        int total_percentage = 100 * finalLetters / totalLetters;
        resOutputItemObject->Set(String::NewSymbol("input_file") , String::New(inputFileArray[i].c_str()));
        resOutputItemObject->Set(String::NewSymbol("output_file") , String::New(outputFile));
        resOutputItemObject->Set(String::NewSymbol("total_letters") , v8::Integer::New(totalLetters));
        resOutputItemObject->Set(String::NewSymbol("final_letters") , v8::Integer::New(finalLetters));
        resOutputItemObject->Set(String::NewSymbol("percentage") , v8::Integer::New(total_percentage));
        /* Set output to output array */
        resOutputItemArray->Set(v8::Integer::New(i), resOutputItemObject);

    }
    /* Close output file */
    closeOutFileStream();

    /* If we have 3 arguments user wants callback */
    if(args.Length() == 3){
        const unsigned argc = 1;
        Local<Function> cb = Local<Function>::Cast(args[2]);

        /* Number of processed files: total args minus two */
        resOutput->Set(String::NewSymbol("processed_files") , v8::Integer::New(inputFileArray.size()));
        resOutput->Set(String::NewSymbol("processed_details") , resOutputItemArray);

        Local<Value> argv[1] = { resOutput };
        cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }

    return scope.Close(Undefined());
}

Handle<Value> minifyString(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("minifyFile"),
      FunctionTemplate::New(minifyFile)->GetFunction());  

  exports->Set(String::NewSymbol("minifyString"),
      FunctionTemplate::New(minifyString)->GetFunction());
}

NODE_MODULE(nativeMinify, init)