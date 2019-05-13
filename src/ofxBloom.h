#ifndef OFX_BLOOM
#define OFX_BLOOM

#include "ofMain.h"
#include "ofxSimpleBlur.h"

class ofxBloom {
	public:

		void allocate(int _width, int _height) {
			target.allocate(_width, _height);
			brightFbo.allocate(_width, _height);
			bloomFbo.allocate(_width, _height);
			blur.allocate(_width, _height);
		}

		void begin() {
			target.begin();
			ofClear(0);
			ofBackground(0);
		}

		void end() {
			target.end();
			
			// limit brightness from Threshold
			brightFbo.begin();

			brightShader.begin();
			brightShader.setUniform1f("threshold", threshold);
			brightShader.setUniformTexture("tex", target.getTexture(), 0);

			target.draw(0, 0);

			brightShader.end();

			brightFbo.end();

			// Blur
			blur.begin();

			brightFbo.draw(0, 0);

			blur.end();

			// Bloom
			bloomFbo.begin();

			bloomShader.begin();
			bloomShader.setUniform1f("strength", strength);
			bloomShader.setUniformTexture("origin", target.getTexture(), 0);
			bloomShader.setUniformTexture("blur", blur.getResult().getTexture(), 1);

			target.draw(0, 0);

			bloomShader.end();

			bloomFbo.end();
		}
		

		// set some parameters
		void setThreshold(float _threshold) {
			if(_threshold < 0)
				threshold = 0.0;
			else if(_threshold > 1.0f)
				threshold = 1.0;
			else 
				threshold = _threshold;
		}
		void setStrength(float _strength) { strength = _strength; }
		void setBlurSize(float _blurSize) { blur.setSize(_blurSize); }

		// draw 
		void draw(float x, float y) {
			bloomFbo.draw(x, y);
		}

		void draw(float x, float y, float w, float h) {
			bloomFbo.draw(x, y, w, h);
		}

		void debugBrightness(float x, float y, float w, float h) {
			brightFbo.draw(x, y, w, h);
		}

		void debugBlur(float x, float y, float w, float h) {
			blur.draw(x, y, w, h);
		}

		// setup
		void setupShaders() {
			blur.setupShader();

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


			// bright shader setup
			stringstream brightFrag;
			brightFrag << "#version 150\n";
			brightFrag << "uniform sampler2DRect tex;\n";
			brightFrag << "uniform float threshold;\n";
			brightFrag << "in vec2 vTexCoord;\n";
			brightFrag << "out vec4 fragColor;\n";
			brightFrag << "void main() {\n";
			brightFrag << "		vec3 texel = texture(tex, vTexCoord).rgb - threshold;\n";
			brightFrag << "		fragColor = vec4(texel, 1.0);\n";
			brightFrag << "}\n";

			brightShader.setupShaderFromSource(GL_VERTEX_SHADER, vertSrc.str());
			brightShader.setupShaderFromSource(GL_FRAGMENT_SHADER, brightFrag.str());
			brightShader.bindDefaults();
			brightShader.linkProgram();
			
			// bloom shader setup
			stringstream bloomFrag;
			bloomFrag << "#version 150\n";
			bloomFrag << "uniform sampler2DRect origin;\n";
			bloomFrag << "uniform sampler2DRect blur;\n";
			bloomFrag << "uniform float strength;\n";
			bloomFrag << "in vec2 vTexCoord;\n";
			bloomFrag << "out vec4 fragColor;\n";
			bloomFrag << "void main() {\n";
			bloomFrag << "		vec4 ori = texture(origin, vTexCoord);\n";
			bloomFrag << "		vec4 blu = texture(blur, vTexCoord) * strength;\n";
			bloomFrag << "		fragColor = vec4(ori.rgb + blu.rgb, 1.0);\n";
			bloomFrag << "}\n";

			bloomShader.setupShaderFromSource(GL_VERTEX_SHADER, vertSrc.str());
			bloomShader.setupShaderFromSource(GL_FRAGMENT_SHADER, bloomFrag.str());
			bloomShader.bindDefaults();
			bloomShader.linkProgram();
		}


	private:
		ofFbo target, brightFbo, bloomFbo;
		ofxSimpleBlur blur;
		ofShader bloomShader, brightShader;
		float threshold, strength; // threshold of brightness
};

#endif
