/***********************************************************************
	Author: => Antony Nelson Daudt Alkmim
	Tic-tac-toe with OpenGL
    Date: 2014/1
***********************************************************************/

#include <stdlib.h>
#include <GL/glut.h>
#include <string>
#include <math.h>

#define WIDTH 500 //LARGURA DA JANELA
#define HEIGHT 500 //ALTURA DA JANELA
#define PI 3.14 //Pi
#define MARGIN_CORTE 10 //distancia do inicio da linha de corte da parede

int janelaGeral;
int campoCordenadas[9][2] = {
	{0,(HEIGHT/3)*2},	{WIDTH/3,(HEIGHT/3)*2},	{(WIDTH/3)*2,(HEIGHT/3)*2},
	{0,HEIGHT/3},		{WIDTH/3,HEIGHT/3},		{(WIDTH/3)*2,HEIGHT/3},
	{0,0},				{WIDTH/3,0},			{(WIDTH/3)*2,0}
};

//campo marcado{0=nenhum;1=X;2=O}
int campoMarcado[9] = {0,0,0,0,0,0,0,0,0}; 

int i;
int VEZ = 1; //1 = X| -1 = O
int Xwins=0,Owins=0;
int linha=0;
int coluna=0;
int calc = 0;
int cortaVencedor = 0; //identifica qual linha desenhar
int vencedor = 0;
int linhasVencedor[8][4] = {
	//cordenadas das linhas para desenhar quando houver vencedor
	//{x0,y0,x,y}
	{ (WIDTH/3)/2 , MARGIN_CORTE , (WIDTH/3)/2 , HEIGHT - MARGIN_CORTE}, // | 1coluna
	{ (WIDTH/2) , MARGIN_CORTE , (WIDTH/2) , HEIGHT - MARGIN_CORTE}, // | 2coluna
	{ WIDTH - (WIDTH/3)/2 , MARGIN_CORTE , WIDTH - (WIDTH/3)/2 , HEIGHT - MARGIN_CORTE}, // | 3coluna
	
	{ MARGIN_CORTE , HEIGHT - (HEIGHT/3)/2, WIDTH - MARGIN_CORTE, HEIGHT - (HEIGHT/3)/2}, // -- 1linha
	{ MARGIN_CORTE , (HEIGHT/2), WIDTH - MARGIN_CORTE, HEIGHT/2}, // -- 2linha
	{ MARGIN_CORTE , (HEIGHT/3)/2, WIDTH - MARGIN_CORTE, (HEIGHT/3)/2}, // -- 3linha

	{ MARGIN_CORTE , HEIGHT - MARGIN_CORTE, WIDTH - MARGIN_CORTE, MARGIN_CORTE}, // [\] transversal
	{ MARGIN_CORTE , MARGIN_CORTE, WIDTH - MARGIN_CORTE, HEIGHT-MARGIN_CORTE}, // [/] transversal
};


/*
 *  Definicao das funcoes
 */
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y); //capta as acoes no teclado
void mouse(int button, int state, int x, int y); //campta as acoes do mouse
void desenhaO(GLfloat x, GLfloat y); //desenha jogadorO
void desenhaX(int x, int y); //Desenha jogadorX
int descobreColuna(int x);
int descobreLinha(int y);
void resetarJogo(void);
void showMenu(void); 
void verificaVencedor(void);
void verPontos(void);



int main(int argc, char** argv){
	glutInit(&argc, argv);
	// glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (WIDTH, HEIGHT);
	glutInitWindowPosition (100, 100); 

	glutCreateWindow ("Jogo-da-Velha");
	showMenu();
	init();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);	
	glutMouseFunc(mouse);
	
	glutMainLoop();
	return 0;
}

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glOrtho (0, WIDTH, 0, HEIGHT, -1 ,1);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	// glPolygonMode(GL_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(15.0);

	//MARGENS DO CAMPO
	glColor3f(1.0, 1.0, 1.0);
	//VERTICAL
	glBegin(GL_LINES); glVertex2i(WIDTH/3,HEIGHT-1);  glVertex2i(WIDTH/3,1); glEnd();
	glBegin(GL_LINES); glVertex2i((WIDTH/3)*2,HEIGHT-1);  glVertex2i((WIDTH/3)*2,1); glEnd();
	//HORIZONTAL
	glBegin(GL_LINES); glVertex2i(1,HEIGHT/3);  glVertex2i(WIDTH-1,HEIGHT/3); glEnd();
	glBegin(GL_LINES); glVertex2i(1,(HEIGHT/3)*2);  glVertex2i(WIDTH-1,(HEIGHT/3)*2); glEnd();

	//DESENHA OS X
	for(i=0; i < 9; i++){
		if(campoMarcado[i] == 1){
			desenhaX(campoCordenadas[i][0],campoCordenadas[i][1]); //passar cordenadas predefinidas em campoCordenadas
		}else if(campoMarcado[i] == -1){
			desenhaO(campoCordenadas[i][0],campoCordenadas[i][1]); //falta desenhar bola
		}
	}

	//verifica se houve vencedor e qual e o vencedor
	if(vencedor==0){
		verificaVencedor();
	}else{
	//DESENHA A LINHA EM CIMA DA SEQUENCIA DE 3 FORMADA
		glColor3f(0.0, 0.0, 1.0);
		glLineWidth(25.0);
		glBegin(GL_LINES);
			glVertex2f(linhasVencedor[cortaVencedor][0],linhasVencedor[cortaVencedor][1]);
			glVertex2f(linhasVencedor[cortaVencedor][2],linhasVencedor[cortaVencedor][3]);
		glEnd();
	}

	//  glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
 }

void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 27: //ESC=27 ENTER=13
			exit(0);
			break;
		case 'R':
		case 'r':
			resetarJogo();
			break;
		case 'V':
		case 'v':
			verPontos();
			break;
		case 'W':
		case 'w':
			printf("\n#########################################\n");
			for(i=0; i<9;i++){
				printf("[%i] - %i\n",i,campoMarcado[i]);
			}
			printf("\n#########################################\n");
			break;
	}
}

void mouse(int button, int state, int x, int y){
	
	if(button == GLUT_LEFT_BUTTON){
		if (state==GLUT_DOWN && vencedor == 0) 
		{  
			linha = descobreLinha(y); 
			coluna = descobreColuna(x);
			
			calc = coluna+(linha*3); //posicao no vetor de campos [0..9]
			if(campoMarcado[calc] == 0){ //Verifica se o campo esta livre
				if(VEZ==1){ //vez do X
					campoMarcado[calc] = VEZ; //marca o campo com X
					VEZ = -1; //passa a vez para o O
				}else if(VEZ==-1){ //vez do O
					campoMarcado[calc] = VEZ; //marca o campo com O
					VEZ = 1; //passa a vez para o X
				}
			}else{
				printf("Campo ja foi marcado!\n");
			} 
		} 
	}
}

void desenhaO(GLfloat x, GLfloat y) 
{ 
	glColor3f(1.0f, 0.0f, 0.0f); 
	GLfloat circle_points = 100.0f; 
	GLfloat angle;
	int raioX=7, raioY=7; 
	glLineWidth(3.0); 
	
	x = x + ((WIDTH/3)/2);
	y = y + ((HEIGHT/3)/2);

	glBegin(GL_LINE_LOOP); 
	for (int i = 0; i < circle_points; i++) 
	{ 
		angle = 2*PI*i/circle_points; 
		glVertex2i((cos(angle)*raioX*10)+x, (sin(angle)*raioY*10)+y); 
	}
	glEnd();

}

void desenhaX(int x, int y){
	glColor3f(0.0f, 1.0f, 0.0f); 
	glLineWidth(5.0);

	//linhas cruzadas
	glBegin(GL_LINES); 
		glVertex2i( x , y ); glVertex2i( x + (WIDTH/3) , y + (HEIGHT/3) );
		glVertex2i( x , y + (HEIGHT/3) ); glVertex2i(x + (WIDTH/3) , y); 
	glEnd(); 
	//glFlush();
}

int descobreColuna(int x){

	if(x < WIDTH/3)//COL_1
		return 0;
	if(x > WIDTH/3 && x < (WIDTH/3)*2) //COL_2
		return 1;
	return 2; //COL_3

}

int descobreLinha(int y){

	if(y < HEIGHT/3)//LINHA_1
		return 0;
	if(y > HEIGHT/3 && y < (HEIGHT/3)*2) //LINHA_2
		return 1;
	return 2; //LINHA_3

}

void resetarJogo(void){
	//limpa todos os campos colocando zero
	for(i = 0; i < 9; i++)
		campoMarcado[i] = 0;
	vencedor = 0;
	cortaVencedor = 0;
}

void showMenu(void){
	
	printf("\n#######################################################################\n");
	printf("#############		JOGO-DA-VELHA - OPENGL		###############\n");
	printf("#######################################################################\n");
	printf("## | MENU | ##\n");
	printf("[R] - Reiniciar Jogo\n");
	printf("[V] - Mostrar Pontuacao\n");
	printf("[ESC] - Sair\n");
	printf("-------------------------------------------------------------------\n");
}

void verPontos(void){
	printf("\n## PONTUACAO : X[%i] x O[%i]\n",Xwins,Owins);
}

void verificaVencedor(void){
	int t = 0;
	int j = 0;

	//verifica 3 casas iguais marcadas na mesma linha
	for(i=0;i<3;i++){//varia entre linhas
		t=0;
		for(j=0; j < 3; j++){
			t += campoMarcado[j+(i*3)];
		}
		//verifica se houve vencedor
		if((t == 3) || (t == -3)){
			vencedor = 1 * t;
			cortaVencedor = i + 3; //linha pra cortar
			printf("\n## VENCEDOR: Jogador[%i] Linha[%i]##\n",vencedor,i);

			//vencedor positivo X ganhou, negativo O ganhou
			if(vencedor > 0) Xwins++;
			else Owins++;

			return;
		}
	}

	//verifica 3 casas iguais marcadas na mesma coluna
	for(i=0;i<3;i++){//varia entre colunas
		t=0;
		for(j=0; j < 3; j++){
			t += campoMarcado[i+(j*3)];
		}
		//verifica se houve vencedor
		if((t == 3) || (t == -3)){
			vencedor = 1 * t;
			cortaVencedor = i; //coluna pra cortar
			printf("\n## VENCEDOR: Jogador[%i] Coluna[%i]##\n",vencedor,i);

			//vencedor positivo X ganhou, negativo O ganhou
			if(vencedor > 0) Xwins++;
			else Owins++;

			return;
		}
	}

	//verifica ganhador na transversal  [\]
	t = campoMarcado[0] + campoMarcado[4] +campoMarcado[8];
	//verifica se houve vencedor
	if((t == 3) || (t == -3)){
		vencedor = 1 * t;
		cortaVencedor = 6; //linha pra cortar
		printf("\n## VENCEDOR: Jogador[%i] Transversal[\\]##\n",vencedor,i);
		//vencedor positivo X ganhou, negativo O ganhou
		if(vencedor > 0) Xwins++;
		else Owins++;
		return;
	}

	//verifica ganhador na transversal  [/]
	t = campoMarcado[2] + campoMarcado[4] + campoMarcado[6];
	//verifica se houve vencedor
	if((t == 3) || (t == -3)){
		vencedor = 1 * t;
		cortaVencedor = 7; //linha pra cortar
		printf("\n## VENCEDOR: Jogador[%i] transversal[/]##\n",vencedor,i);
		//vencedor positivo X ganhou, negativo O ganhou
		if(vencedor > 0) Xwins++;
		else Owins++;
	}

}