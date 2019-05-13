// 2019/05/12
// made by shunji kiuchi
//

#ifndef OFX_SIMPLE_BLUR
#define OFX_SIMPLE_BLUR

#include "ofMain.h"

class ofxSimpleBlur {
	public:

		void allocate(int _width, int _height) {
			target.allocate(_width, _height);
			onePassFbo.allocate(_width, _height);
			result.allocate(_width, _height);
		}

		void begin() {
			target.begin();
			ofClear(0);
			ofBackground(0);
		}

		void end() {
			target.end();

			// horizontal blur
			onePassFbo.begin();

			blurShader.begin();
			blurShader.setUniform1f("blurSize", size);
			blurShader.setUniformTexture("tex", target.getTexture(), 0);
			blurShader.setUniform2f("direction", 1.0, 0.0);

			target.draw(0, 0);

			blurShader.end();
			onePassFbo.end();

			// vertical
			result.begin();

			blurShader.begin();
			blurShader.setUniform1f("blurSize", size);
			blurShader.setUniformTexture("tex", onePassFbo.getTexture(), 0);
			blurShader.setUniform2f("direction", 0.0, 1.0);

			onePassFbo.draw(0, 0);

			blurShader.end();
			result.end();
		}

		void draw(int x, int y) {
			result.draw(x, y);
		}

		void draw(int x, int y, int w, int h) {
			result.draw(x, y, w, h);
		}

		void setupShader() {
			stringstream vertSrc;
			vertSrc << "#version 150\n";
			vertSrc << "uniform mat4 modelViewProjectionMatrix;\n";
			vertSrc << "in vec4 position;\n";
			vertSrc << "in vec2 texcoord;\n";
			vertSrc << "out vec2 vTexCoord;\n";
			vertSrc << "void main() {\n";
			vertSrc << "	gl_Position = modelViewProjectionMatrix * position;\n";
			vertSrc << "	vTexCoord = texcoord;\n";
			vertSrc << "}\n";

			stringstream fragSrc;
			fragSrc << "#version 150\n";
			fragSrc << "uniform sampler2DRect tex;\n";
			fragSrc << "uniform vec2 direction;\n";
			fragSrc << "uniform float blurSize;\n";
			fragSrc << "in vec2 vTexCoord;\n";
			fragSrc << "\n";
			fragSrc << "out vec4 fragColor;\n";
			fragSrc << "\n";
			fragSrc << "void main() {\n";
			fragSrc << "	const int N = 16;\n";
			fragSrc << "	float delta = blurSize / float(N);\n";
			fragSrc << "	vec3 color = texture(tex, vTexCoord).rgb;\n";
			fragSrc << "	for(int i=0; i<N; i++) {\n";
			fragSrc << "		vec2 d = direction * float(i) * delta;\n";
			fragSrc << "		color += texture(tex, vTexCoord + d).rgb;\n";
			fragSrc << "		color += texture(tex, vTexCoord - d).rgb;\n";
			fragSrc << "	}\n";
			fragSrc << "	color /= float(N) * 2.0 + 1.0;\n";
			fragSrc << "	fragColor = vec4(color, 1.0);\n";
			fragSrc << "}\n";

			blurShader.setupShaderFromSource(GL_VERTEX_SHADER, vertSrc.str());
			blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragSrc.str());
			blurShader.bindDefaults();
			blurShader.linkProgram();
		}

		void debugFbo() {
			int w = target.getWidth() / 2;
			int h = target.getHeight() / 2;
			target.draw(0, 0, w, h);
			onePassFbo.draw(w, 0, w, h);
			result.draw(0, h, w, h);
		}

		ofFbo getResult() { return result; }
		ofFbo getTarget() { return target; }

		void setSize(float _size) { size = _size; }
		float getSize() { return size; }
	private:
		float size;
		ofShader blurShader;
		ofFbo target, onePassFbo, result;

};

#endif