const char* program1_cl = 
"\n"
"#define a1 (-1.586134342)\n"
"#define a2 (-0.05298011854)\n"
"#define a3 (0.8829110762)\n"
"#define a4 (0.4435068522)\n"
"#define k1 1.1496043988602418\n"
"#define k2 (1/1.1496043988602418)\n"
"#define x(i, j) img_start[(i)*m+(j)]\n"
"\n"
"__kernel\n"
"void pone (\n"
"int n,\n"
"int m,\n"
"__global float* img,\n"
"__global float* tmp,\n"
"int p\n"
")\n"
"{\n"
"int i;\n"
"float* img_start;\n"
"float* tmp_start;\n"
"\n"
"i = get_group_id(0);\n"
"img_start = img + p*m*n;\n"
"tmp_start = tmp + p*m*n;\n"
"/*\n"
"int j;\n"
"for (j=1; j<m-2; j+=2) {\n"
"x(i, j) += a1*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, m-1) += 2*a1*x(i, m-2);\n"
"// Update 1\n"
"for (j=2; j<m; j+=2) {\n"
"x(i, j) += a2*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, 0) += 2*a2*x(i, 1);\n"
"// Predict 2\n"
"for (j=1; j<m-2; j+=2) {\n"
"x(i, j) += a3*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, m-1) += 2*a3*x(i, m-2);\n"
"// Update 2\n"
"\n"
"for (j=2; j<m; j+=2) {\n"
"x(i, j) += a4*(x(i, j-1) + x(i, j+1));\n"
"}\n"
"x(i, 0) += 2*a4*x(i, 1);\n"
"*/\n"
"}\n"
;
