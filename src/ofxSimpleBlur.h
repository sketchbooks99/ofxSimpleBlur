// 2019/05/12
// made by shunji kiuchi
//

#include "ofMain.h"

class ofxSimpleBlur {
	public:
		ofxSimpleBlur();
		~ofxSimpleBlur();

		void allocate(int _width, int _height) {
			target.allocate(_width, _height);
			onePassFbo.allocate(_width, _height);
			result.allocate(_width, _height);
		}

		void begin() {
			target.begin();
			ofClear(0);
			ofBackground(0);OB
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
	
			stringstream bloomFrag;
			bloomFrag << "#version 150\n";
			bloomFrag << "uniform sampler2DRect origin;\n";
			bloomFrag << "uniform sampler2DRect result;\n";
			bloomFrag << "uniform vec2 resolution;\n";
			bloomFrag << "uniform float strength;\n";
			bloomFrag << "in vec2 vTexCoord;\n";
			bloomFrag << "out vec4 fragColor;\n";
			bloomFrag << "";
			bloomFrag << "void main() {\n";
			bloomFrag << "	vec2 p = (vTexCoord * 2.0 - resolution) / min(resolution.x, resolution.y);\n";
			bloomFrag << "	vec2 rOffset = vec2(2.0, 0.0);\n";
			bloomFrag << "	vec2 bOffset = vec2(4.0, 0.0);\n";
			bloomFrag << "	vec4 ori = texture(origin, vTexCoord);\n";
			bloomFrag << "	vec4 res = texture(result, vTexCoord);\n";
			bloomFrag << "	float rOri = texture(origin, vTexCoord + rOffset).r;\n";
			bloomFrag << "	float rRes = texture(result, vTexCoord + rOffset).r;\n";
			bloomFrag << "	float bOri = texture(origin, vTexCoord + bOffset).b;\n";
			bloomFrag << "	float bRes = texture(result, vTexCoord + bOffset).b;\n";
			bloomFrag << "	vec3 destColor = vec3(rOri, ori.g, bOri) + vec3(rRes, res.g, bRes) * strength;\n";
			bloomFrag << "	fragColor = vec4(destColor, 1.0);\n";
			bloomFrag << "}\n";

			bloomShader.setupShaderFromSource(GL_VERTEX_SHADER, vertSrc.str());
			bloomShader.setupShaderFromSource(GL_FRAGMENT_SHADER, bloomFrag.str());
			bloomShader.bindDefaults();
			OAbloomShader.linkProgram();
		}

		void bloomDraw(int x, int y) {
			bloomShader.begin();
			bloomShader.setUniformTexture("origin", target.getTexture(), 0);
			bloomShader.setUniformTexture("result", result.getTexture(), 1);
			bloomShader.setUniform1f("strength", strength);

			target.draw(x, y);

			bloomShader.end();
		}

		void bloomDraw(int x, int y, int w, int h) {
			bloomShader.begin();
			bloomShader.setUniformTexture("origin", target.getTexture(), 0);
			bloomShader.setUniformTexture("result", result.getTexture(), 1);
			bloomShader.setUniformaf("strength", strength);

			target.draw(x, y, w, h);

			bloomShader.end();
		}

		void debugFbo() {
			int w = target.getWidth() / 2;
			int h = target.getHeight() / 2;
			target.draw(0, 0, w, h);
			onePassFbo.draw(w, 0, w, h);
			result.draw(0, h, w, h);
			bloomDraw(w, h, w, h);
		}

		ofFbo getResult() { return result; }
		ofFbo getTarget() { return target; }

		void setSize(float _size) { size = _size; }
		void setStrength(float _strength) { strength = _strength; }OB
	private:
		float size, strength;
		ofShader blurShader, bloomShader;
		ofFbo target, onePassFbo, result;OA

};
