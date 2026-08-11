#ifndef SHADERS_BAYER_GLSL
#define SHADERS_BAYER_GLSL
// 4x4 Bayer Matrix lookup table packed into an array
// credit: [wikipedia - Ordered Dithering](https://en.wikipedia.org/wiki/Ordered_dithering)
const float bayer_matrix_8x8[8*8] = float[](
	0.0078125, 0.7578125, 0.1953125, 0.9453125, 0.0546875, 0.8046875, 0.2421875, 0.9921875,
	0.5078125, 0.2578125, 0.6953125, 0.4453125, 0.5546875, 0.3046875, 0.7421875, 0.4921875,
	0.1328125, 0.8828125, 0.0703125, 0.8203125, 0.1796875, 0.9296875, 0.1171875, 0.8671875,
	0.6328125, 0.3828125, 0.5703125, 0.3203125, 0.6796875, 0.4296875, 0.6171875, 0.3671875,
	0.0390625, 0.7890625, 0.2265625, 0.9765625, 0.0234375, 0.7734375, 0.2109375, 0.9609375,
	0.5390625, 0.2890625, 0.7265625, 0.4765625, 0.5234375, 0.2734375, 0.7109375, 0.4609375,
	0.1640625, 0.9140625, 0.1015625, 0.8515625, 0.1484375, 0.8984375, 0.0859375, 0.8359375,
	0.6640625, 0.4140625, 0.6015625, 0.3515625, 0.6484375, 0.3984375, 0.5859375, 0.3359375
);
float bayer8(vec2 uv) { return float(bayer_matrix_8x8[ROW_MAJOR_IDX2D((ivec2(uv)%8),8,8)]); }

const float bayer_matrix_4x4[4*4] = float[](
	0.03125, 0.53125, 0.15625, 0.65625,
	0.78125, 0.28125, 0.90625, 0.40625,
	0.21875, 0.71875, 0.09375, 0.59375,
	0.96875, 0.46875, 0.84375, 0.34375
);
float bayer4(vec2 uv) { return float(bayer_matrix_4x4[ROW_MAJOR_IDX2D((ivec2(uv)%4),4,4)]); }

const float bayer_matrix_2x2[2*2] = float[](
	0.125, 0.625,
	0.875, 0.375
);

float bayer2(vec2 uv) { return float(bayer_matrix_2x2[ROW_MAJOR_IDX2D((ivec2(uv)%2),2,2)]); }
#endif //SHADERS_BAYER_GLSL
