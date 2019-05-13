# ofxSimpleBlur

## 注意事項
```c++
ofxSimpleBlur blur;
blur.allocate(x, y);
blur.setupShaders();

ofxBloom bloom;
bloom.allocate(x, y);
bloom.setupShaders();
```
を忘れずに行ってください。

## 使い方
exampleはwindowsで作成したので、windows版しかありません。addonは環境関係なく動くと思います。
exampleの中にBloomの使い方は書いてあるので、それを参考にしてください。

![result](https://github.com/sketchbooks99/ofxSimpleBlur/blob/master/result.PNG)
