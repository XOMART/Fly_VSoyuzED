// FlyHtmlWriterTest.cpp :
#include "stdafx.h"
#include <direct.h>

// ����������� ���������� FlyHtmlWriter
#define IMPORT_FLY_HTML_WRITER
#include <FlyHtmlWriter.h>

void BuildIDTable(Z�HtmlItem& I);
void BuildNuTable(Z�HtmlItem& I);
void FillPredVitTable(Z�HtmlItem& I);

void main()
{
	// "��������" ��� �������� HTML-���������
	ZCHtmlBuilder HB;

	// ������������ ������� ������ CSS 
	HB.S.cssLocation = 1;
	HB.BuildStyles();
	// ������������� � ��������� ������ ����� ��� ���� ����������
	// ������� Fly
	//HB.UseDefaultStyles();

	// ������������� �������� ��������� 
	HB.Init("�������� ������ ��� ��������� Html-����������������",
	"������� ���������� ��������", 1, nullptr);

	// ������� ��������� ��������� ��������� ���������, ������� ������ � 
	// ������� ������������� HB.Init - "������� ���������� ��������".
	// ���� �� ��������� ������������ � Init, �� ���������� ���������
	// ���������� ���������� �� � 0.

	// ������ �������������� ����� 
	HB(1).n = "hr";

	// ������� � ��������� ������� ������� ������
	BuildIDTable(HB(3));

	// �������������� ����� � ������ ������
	HB(4).n = "hr";
	HB(5).n = "br";

	// �� + ��� � ���� �������
	BuildNuTable(HB(6));

	// ������ ������
	HB(7) = "br";

	// �������� � ��������� �������� ����� - ������� ��������� ����� ����
	// � ���� <p> ������� ��� ��� ���� <b><i><span>
	// � �������� ����� ����� �����:
	//		<p>
	//			<p><i><span>
	//				����� ��������� ���� �������������� . . .
	//			</span></i></b>
	//		</p> 
	HB(8).n = "p";
	HB(8).C(0) = "b";
	HB(8).C(1) = "i";
	HB(8).C(2) = "span";
	// ��������  ���� �� ������� ����� ������� "style" �� ��������� 
	// "font-size: 12pt; color: #ff0000 . . ."
	// � Html ������:
	//			<p><i><span style = "font-size: 12pt; color: #ff0000 . . .">
	//				����� ��������� ���� �������������� . . .
	//			</span></i></b>
	HB(8).C(2)[0] << "style\nfont-size: 12pt; color: #ff0000; text-indent:90pt; line-height:20pt";
	HB(8).txt = 
"����� ��������� ���� �������������� ��� ��� ������������� \
������� �������, ����� ���������� ��� �� ��� ������������ ��� ������������ \
���� ����� ��� ���������� �������� ���������. ��������� ����� ��������� \
������������� �������  =0. ����  <0, �� ��� ������������ � ����� ��������� ���� \
��������������, ���� >0, ��� ��������� �� ����� ��������� ���� ��������������. \
����������� ����� ����������� �������������� �� ������ ���� �������������� \
������� ���������������� ��������� ������������ �������� ���.";

	// �������� ������� � ������������ ���������
	FillPredVitTable(HB(9));

	HB(10).n = "p";
	HB(10).txt =
"����� ��������� ���� �������������� ��� ��� ������������� \
������� �������, ����� ���������� ��� �� ��� ������������ ��� ������������ \
���� ����� ��� ���������� �������� ���������. ��������� ����� ��������� \
������������� �������  =0. ����  <0, �� ��� ������������ � ����� ��������� ���� \
��������������, ���� >0, ��� ��������� �� ����� ��������� ���� ��������������. \
����������� ����� ����������� �������������� �� ������ ���� �������������� \
������� ���������������� ��������� ������������ �������� ���.";

	HB(11).n = "hr";

	HB(12).n = "div";
	// ������� HB(12)(0) ����� ��������� ��� �������� HB(12)
	HB(12)(0).n = "div";
	// ��� ������� ��������� �� ����� ������ �� ������ ���� ���������
	// ������� padding
	HB(12)(0)[0] << "style \n padding: 5pt 36pt 5pt 72pt";
	HB(12)(0).C(0) = "b";
	HB(12)(0).C(1) = "i";
	HB(12)(0).txt =
"���� ������� ������� � ��������� �������� ����� HTML, ��� ������ �������� \
�� �������� � �������� ����� ���� ��� - ��������.HTML ���� - ��� ������, ���� \
������� ������ ���� ���������� ����� ������ ����������� ��� - �������!���� ����, \
��� �� ����(� ������ ����), ���������� �������, ����� ��� � ��������� ����� \
������� ����� \"�����������\" �������������..� ���� ����� �� ��������� \
�������������� ��������� ����������� �������� � �����.";

	// ������ ��������� ������� ��� HB(12)
	HB(12)(1).n = "div";
	HB(12)(1)[0] << "style \n padding: 5pt 36pt 5pt 72pt";
	HB(12)(1).txt =
"���� ������� ������� � ��������� �������� ����� HTML, ��� ������ �������� \
�� �������� � �������� ����� ���� ��� - ��������.HTML ���� - ��� ������, ���� \
������� ������ ���� ���������� ����� ������ ����������� ��� - �������!���� ����, \
��� �� ����(� ������ ����), ���������� �������, ����� ��� � ��������� ����� \
������� ����� \"�����������\" �������������..� ���� ����� �� ��������� \
�������������� ��������� ����������� �������� � �����.";

	// �������������� ����� � ������ ������
	HB(13).n = "hr";
	HB(14).n = "br";

	// ��������� �������� � �������
	// ������ ������ ���������� ������ �������� � ���������.
	// ������� ������� ����������� ����� 16-� �� ������ � ����� ������ 
	// � ������� ��������� - 15
	HB << Z�HtmlItem("p", 0,
"����� ��������� ���� �������������� ��� ��� ������������� \
������� �������, ����� ���������� ��� �� ��� ������������ ��� ������������ \
���� ����� ��� ���������� �������� ���������. ��������� ����� ��������� \
������������� �������  =0. ����  <0, �� ��� ������������ � ����� ��������� ���� \
��������������, ���� >0, ��� ��������� �� ����� ��������� ���� ��������������. \
����������� ����� ����������� �������������� �� ������ ���� �������������� \
������� ���������������� ��������� ������������ �������� ���.");

	// ���������� ���� �������� � ���� Html
	// �� ����� ������ ��� ���������� ���������� � ����������� ������ 
	mkdir(".\\_����������");
	HB.Flash(".\\_����������\\������");
	system(".\\_����������\\������.html");

	// . . . ���-�� ��� . . .
}

//-----------------------------------------------------------------------------
void BuildIDTable(Z�HtmlItem& I)
{
	// ������� � ����������� ��������
	// ������ ������� ��� ��� table
	I.n = "table";
	// � �������� �������� ��������� ������ �������� ������ � CSS-�����.
	// � ��������������� CSS-����� ��� � ��������� Html-��������� ������
	// ���� ����� ����� �����:
	//		table.FlyID {
	//			font - family : 'Times New Roman';
	//			. . .
	//		}
	I[0] << "class\n FlyID";

	// � ������� ������ ������� ������ - tr
	I(0).n = "tr";
	// � ������ ������ ������ ������� ������ ��������� - th
	I(0)(0).n = "th";
	// ����� ���� ������
	I(0)(0).txt = "����� ������ �������������";
	// ��������� ������ ������ � � �����
	I(0)(1).n = "td";
	I(0)(1).txt = "101";

	// ��������� ������ �������
	I(1).n = "tr";
	I(1)(0).n = "th";
	I(1)(0).txt = "����� ����� �������������";
	I(1)(1).n = "td";
	I(1)(1).txt = "234";

	// ����� �� �� ��������
	I(2).n = "tr";
	I(2)(0).n = "th";
	I(2)(0).txt = "��� ��������������";
	I(2)(1).n = "td";
	I(2)(1).txt = "30.0";
	I(2)(2).n = "td";
	I(2)(2).txt = "���";
}

//-----------------------------------------------------------------------------
void BuildNuTable(Z�HtmlItem& I)
{
	// ������� � ����������� �������� ����������� � �������� Html-���������
	// I, ������ �� ������� ��������� � �������� ��������� ��������� �������
	I.n = "table";
	// !!!! ���������� ������ ��� ������ � ��������� ��������
	I[0] << "class\n NU";

	// ������ ������ ��������� � �������
	I(0).n = "tr";
	// ������ ������ ������ - ��� ������ ������������� �� 5 ����� 
	// ����������� �����
	I(0)(0).n = "th";
	// ���������� ����������� ����� - ����������� ������ "�����"
	// 5-� ������� ��������� �����
	I(0)(0)[0] << "colspan \n 5";
	// ����� ������
	I(0)(0).txt = "�� ��-23";

	// ��������� ������ ����������� ���������� . . . 
	I(1).n = "tr";
	// ������ � ������������ ������ ������ ������ ����� ������� ������� td
	I(1)(0).n = "td";
	I(1)(0)[0] << "colspan \n 5";
	// �� ����������� ���� ��� ����������� ��� ���� �������
	// � ������ "style" 
	I(1)(0)[1] << "style \n text-align : left";
	I(1)(0).txt = "�����-1 ���-0 ���-0 ��-1  ����� ���-10<br>\
����  07.05.2014   �����  18:11:19.5790   ����� 3084";

	I(2).n = "tr";
	I(2)(0).n = "th";
	I(2)(0)[0] << "colspan \n 2";
	I(2)(0).txt = "�����, ��";
	I(2)(1).n = "th";
	I(2)(1).txt = "S<sub>���</sub>";
	I(2)(2).n = "th";
	I(2)(2).txt = "<font face=\"symbol\">K<font>";
	I(2)(3).n = "th";
	I(2)(3).txt = "";

	I(3).n = "tr";
	I(3)(0).n = "td";
	I(3)(0)[0] << "colspan \n 2";
	I(3)(0).txt = "872";
	I(3)(1).n = "td";
	I(3)(1).txt = "0.0001425";
	I(3)(2).n = "td";
	I(3)(2).txt = "0.00619";
	I(3)(3).n = "td";
	I(3)(3).txt = "";

	// ����� �� �� ��������: ������, ������, �������� � ����� 
	I(5).n = "tr";
	I(5)(0).n = "th";
	I(5)(0)[0] << "colspan \n 2";
	I(5)(0).txt = "";
	I(5)(1).n = "th";
	I(5)(1).txt = "���-2000";
	I(5)(2).n = "th";
	I(5)(2).txt = "���    ";
	I(5)(3).n = "th";
	I(5)(3).txt = "���-����";

	I(6).n = "tr";
	I(6)(0).n = "th";
	I(6)(0).txt = "X,<br>Y,<br>Z,<br>V<sub>x</sub>,<br>V<sub>y</sub>,<br>V<sub>z</sub>,";
	I(6)(1).n = "th";
	I(6)(1).txt = "��<br>��<br>��<br>�/���<br>�/���<br>�/���";
	I(6)(2).n = "td";
	I(6)(2).txt = "-6538.452764<br>-3213.890898<br>9.068114<br>1276.312730<br>-2577.384662<br>6816.735169";
	I(6)(3).n = "td";
	I(6)(3).txt = "-2863.602400<br>6699.277780<br>0.000000<br>-2151.963060<br>-919.834831<br>6818.633500";
	I(6)(4).n = "td";
	I(6)(4).txt = "-6528.012234<br>-3235.057611<br>0.000000<br>1275.059023<br>-2572.979970<br>6818.633500";

	I(7).n = "tr";
	I(7)(0).n = "th";
	I(7)(0)[0] << "colspan \n 2";
	I(7)(0).txt = "";
	I(7)(1).n = "th";
	I(7)(1).txt = "���-2000";
	I(7)(2).n = "th";
	I(7)(2).txt = "���-����";
	I(7)(3).n = "th";
	I(7)(3).txt = "";

	I(8).n = "tr";
	I(8)(0).n = "th";
	I(8)(0).txt = "a,<br>e<br>i,<br><font face=\"symbol\">W</font>,<br><font face=\"symbol\">w</font>, <br>u,";
	I(8)(1).n = "th";
	I(8)(1).txt = "��<br> <br>����<br>����<br>����<br>����";
	I(8)(2).n = "td";
	I(8)(2).txt = "7289.548306<br>0.000536240<br>67.124471<br>206.145732<br>359.804627<br>0.077401";
	I(8)(3).n = "td";
	I(8)(3).txt = "7289.548306<br>0.000536240<br>67.162195<br>206.361428<br>359.727226<br>0.000000";

	I(9).n = "tr";
	I(9)(0).n = "th";
	I(9)(0)[0] << "colspan \n 5";
	I(9)(0).txt = "���-1";

	// ���������� ����� ���
	I(10).n = "tr";
	I(10)(0).n = "td";
	I(10)(0)[0] << "colspan \n 5";
	I(10)(0)[1] << "style \n text-align : left";
	I(10)(0).txt = "��� ��� - 2 (02x00)<br>\
��� ��� - 1 ��� ��� - 0 ���  �� - 0<br>\
��� ��� - 0 ��� ��� - 0 ��� ��� - 0";
}

//-----------------------------------------------------------------------------
void FillPredVitTable(Z�HtmlItem& I)
{
	// ������� � ����������� ��������
	// ��� �������� ���������� ���� �����������, �� ����������� 
	// Html-�������� � ������ ����.
	I.n = "table";
	I[0] << "class\n predByVit";

	I(1).n = "tr";
	I(1)(0).n = "th";
	I(1)(0)[0] << "colspan \n 2";
	I(1)(0).txt = "�����<br>����<br>�����";

	I(1)(1).n = "th";
	I(1)(1).txt = "101<br>01.01.2011<br>00:00:00,0000";
	I(1)(2).n = "th";
	I(1)(2).txt = "101<br>01.01.2011<br>00:15:00,0000";
	I(1)(3).n = "th";
	I(1)(3).txt = "101<br>01.01.2011<br>00:30:00,0000";
	I(1)(4).n = "th";
	I(1)(4).txt = "101<br>01.01.2011<br>00:45:00,0000";

	I(2).n = "tr";
	I(2)(0).n = "th";
	I(2)(0).txt = "m,";
	I(2)(1).n = "th";
	I(2)(1).txt = "��";
	I(2)(2).n = "td";
	I(2)(2).txt = "1200.000";
	I(2)(3).n = "td";
	I(2)(3).txt = "1200.000";
	I(2)(4).n = "td";
	I(2)(4).txt = "1200.000";
	I(2)(5).n = "td";
	I(2)(5).txt = "1200.000";

	I(3).n = "tr";
	I(3)(0).n = "th";
	I(3)(0)[0] << "colspan \n 6";
	I(3)(0)[1] << "style \n text-align: center";
	I(3)(0).txt = "��� 1975";

	I(4).n = "tr";
	I(4)(0).n = "th";
	// !!!! Html-�������� � ������ ���� !!!!
	// ������ "Vx" ����� "V<sub>x</sub>", ��� ��� ��� �������������� ������
	// ���������� V !!!!
	I(4)(0).txt = "X,<br>Y,<br>Z,<br>V<sub>x</sub>,<br>V<sub>y</sub>,<br>V<sub>z</sub>,<br>a,<br>e<br>i,<br><font face=\"symbol\">W</font>,<br><font face=\"symbol\">w</font>, <br>u,";

	I(4)(1).n = "th";
	I(4)(1).txt = "��<br>��<br>��<br>�/���<br>�/���<br>�/���<br>��<br> <br>����<br>����<br>����<br>����";

	I(4)(2).n = "td";
	I(4)(2).txt = 
"-1850.446018<br>-6782.021383<br>1073.904577<br>\
3924.239246<br>-59.750623<br>6375.199978<br>\
7111.00000000<br>0.000200000<br>59.869340<br>\
249.651917<br>120.055469<br>10.055469";

	I(4)(3).n = "td";
	I(4)(3).txt = 
"-1850.446018<br>-6782.021383<br>1073.904577<br>\
3924.239246<br>-59.750623<br>6375.199978<br>\
7111.00000000<br>0.000200000<br>59.869340<br>\
249.651917<br>120.055469<br>10.055469";

	I(4)(4).n = "td";
	I(4)(4).txt = 
"-1850.446018<br>-6782.021383<br>1073.904577<br>\
3924.239246<br>-59.750623<br>6375.199978<br>\
7111.00000000<br>0.000200000<br>59.869340<br>\
249.651917<br>120.055469<br>10.055469";

	I(4)(5).n = "td";
	I(4)(5).txt = 
"-1850.446018<br>-6782.021383<br>1073.904577<br>\
3924.239246<br>-59.750623<br>6375.199978<br>\
7111.00000000<br>0.000200000<br>59.869340<br>\
249.651917<br>120.055469<br>10.055469";
}

//-----------------------------------------------------------------------------