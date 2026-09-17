#include "MyForm.h"
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>

using std::string;
using std::map;
using std::vector;
using std::pair;
using std::max;

using namespace System;
using namespace System::Windows::Forms;

// Функция для безопасного создания читаемой строки из UTF-8 текста кода
String^ ToUtf8String(const char* c_str) {
	int len = static_cast<int>(strlen(c_str)); // ИСПРАВЛЕНО: Явное приведение для устранения warning
	array<Byte>^ bytes = gcnew array<Byte>(len);
	System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)c_str), bytes, 0, len);
	// ИСПРАВЛЕНО: Вместо двойного двоеточия используется стрелочка -> для вызова GetString
	return System::Text::Encoding::UTF8->GetString(bytes);
}

string toStdString(String^ s) {
	using System::Runtime::InteropServices::Marshal;
	if (s == nullptr) return "";
	IntPtr ptr = Marshal::StringToHGlobalAnsi(s);
	string res((char*)ptr.ToPointer());
	Marshal::FreeHGlobal(ptr);
	return res;
}

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	spoLab1::MyForm form;
	Application::Run(% form);
	return 0;
}

System::Void spoLab1::MyForm::btnAnalyze_Click(System::Object^ sender, System::EventArgs^ e) {

	map<string, int> operators;
	map<string, int> operands;

	string command = "C:\\php\\php.exe tokenizer.php input.php";

	FILE* pipe = _popen(command.c_str(), "r");
	if (!pipe) {
		MessageBox::Show(ToUtf8String("Ошибка: Не удалось запустить PHP-скрипт!"), ToUtf8String("Ошибка"), MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	char buffer[256];
	string prevTokenType = "";
	string prevTokenContent = "";
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		string line(buffer);
		if (!line.empty() && line.back() == '\n') line.pop_back();
		size_t pos = line.find("===");

		if (pos != string::npos) {
			string tokenType = line.substr(0, pos);
			string tokenContent = line.substr(pos + 3);

			if (tokenType == "T_WHITESPACE" ||
				tokenType == "T_COMMENT" ||
				tokenType == "T_DOC_COMMENT" ||
				tokenType == "T_OPEN_TAG" ||
				tokenType == "T_CLOSE_TAG")
			{
				//просто игнор, незначимые токены
				continue; 
			}
			
			if (tokenType == "CHARACTER")
			{
				if (tokenContent == "(") 
				{
					if (prevTokenType != "T_STRING" && 
						prevTokenType != "T_IF" &&
						prevTokenType != "T_ELSEIF" &&
						prevTokenType != "T_SWITCH" &&
						prevTokenType != "T_FOR" &&
						prevTokenType != "T_FOREACH" &&
						prevTokenType != "T_WHILE"
						) {
						operators["( )"]++;
					}
				}
				else if (tokenContent == "[") {
					operators["[ ]"]++;
				}
				else if (tokenContent == ")" || 
					tokenContent == "{" || 
					tokenContent == "}" || 
					tokenContent == "]" || 
					tokenContent == "," || 
					tokenContent == ":" 
					//tokenContent == ";"
					) {
					//игнор
				}
				else {
					operators[tokenContent]++;
				}
			}
			else if (tokenType == "T_STRING") {
				if (tokenContent == "null") {
					operands[tokenContent]++;
				}
				else if (tokenContent == "int" ||
					tokenContent == "float") {
					//типизация, игнор
				}
				else {
					//ветка для функций
					if (prevTokenType == "T_FUNCTION") {
						//объявление фукции, игнор
					}
					else {
						//вызов функции, вполне себе оператор
						operators[tokenContent + "(...)"]++;
					}
				}
			}
			else if (tokenType == "T_VARIABLE" ||
				tokenType == "T_LNUMBER" ||
				tokenType == "T_DNUMBER" ||
				tokenType == "T_CONSTANT_ENCAPSED_STRING") {
				operands[tokenContent]++;
			}
			else if (tokenType == "T_FUNCTION" || 
				tokenType == "T_ARRAY" || 
				tokenType == "T_ELSEIF" || 
				tokenType == "T_ELSE" ||
				tokenType == "T_DEFAULT" ||
				tokenType == "T_CASE"
				) {
				//игнор
			} 
			else {
				operators[tokenContent]++;
			}

			prevTokenContent = tokenContent;
			prevTokenType = tokenType;
		}
	}
	_pclose(pipe);

	vector<pair<string, int>> operands_vec(operands.begin(), operands.end());
	vector<pair<string, int>> operators_vec(operators.begin(), operators.end());

	int n = static_cast<int>(operands_vec.size());
	int m = static_cast<int>(operators_vec.size());
	int max_rows = max(n, m);

	int total_operands = 0;
	for (const auto& p : operands_vec) total_operands += p.second;

	int total_operators = 0;
	for (const auto& p : operators_vec) total_operators += p.second;

	// --- НАСТРОЙКА И ЗАПОЛНЕНИЕ ТАБЛИЦЫ ---
	dataGridView1->Rows->Clear();
	dataGridView1->ColumnCount = 6;
	dataGridView1->AllowUserToAddRows = false;

	// Столбцы поменялись местами согласно требованию
	dataGridView1->Columns[0]->HeaderText = ToUtf8String("№ опн");
	dataGridView1->Columns[1]->HeaderText = ToUtf8String("Операнд");
	dataGridView1->Columns[2]->HeaderText = ToUtf8String("Кол-во");
	dataGridView1->Columns[3]->HeaderText = ToUtf8String("№ опр");
	dataGridView1->Columns[4]->HeaderText = ToUtf8String("Оператор");
	dataGridView1->Columns[5]->HeaderText = ToUtf8String("Кол-во");

	for (int i = 0; i < max_rows; ++i) {
		int rowIndex = dataGridView1->Rows->Add();

		if (i < n) {
			dataGridView1->Rows[rowIndex]->Cells[0]->Value = i + 1;
			dataGridView1->Rows[rowIndex]->Cells[1]->Value = ToUtf8String(operands_vec[i].first.c_str());
			dataGridView1->Rows[rowIndex]->Cells[2]->Value = operands_vec[i].second;
		}
		if (i < m) {
			dataGridView1->Rows[rowIndex]->Cells[3]->Value = i + 1;
			dataGridView1->Rows[rowIndex]->Cells[4]->Value = ToUtf8String(operators_vec[i].first.c_str());
			dataGridView1->Rows[rowIndex]->Cells[5]->Value = operators_vec[i].second;
		}
	}

	int totalRowIndex = dataGridView1->Rows->Add();
	System::Drawing::Font^ currentFont = dataGridView1->Font;
	dataGridView1->Rows[totalRowIndex]->DefaultCellStyle->Font = gcnew System::Drawing::Font(currentFont, System::Drawing::FontStyle::Bold);

	dataGridView1->Rows[totalRowIndex]->Cells[0]->Value = ToUtf8String("Уник: ") + n;
	dataGridView1->Rows[totalRowIndex]->Cells[1]->Value = "";
	dataGridView1->Rows[totalRowIndex]->Cells[2]->Value = ToUtf8String("Всего: ") + total_operands;
	dataGridView1->Rows[totalRowIndex]->Cells[3]->Value = ToUtf8String("Уник: ") + m;
	dataGridView1->Rows[totalRowIndex]->Cells[4]->Value = "";
	dataGridView1->Rows[totalRowIndex]->Cells[5]->Value = ToUtf8String("Всего: ") + total_operators;

	// --- РАСЧЕТ МЕТРИК ХОЛСТЕДА ---
	int η = n + m;
	int N = total_operands + total_operators;
	double V = (η > 0) ? N * log2(η) : 0;

	String^ metricsText = ToUtf8String("--- МЕТРИКИ ХОЛСТЕДА ---\n") +
		ToUtf8String("Словарь программы (η): ") + η + ToUtf8String(" (уник. операторов: ") + m + ToUtf8String(", уник. операндов: ") + n + ToUtf8String(")\n") +
		ToUtf8String("Длина программы (N):   ") + N + ToUtf8String(" (всего операторов: ") + total_operators + ToUtf8String(", всего операндов: ") + total_operands + ToUtf8String(")\n") +
		ToUtf8String("Объем программы (V):   ") + V.ToString("F2") + ToUtf8String(" бит");

	lblMetrics->Text = metricsText;
}
