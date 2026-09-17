#pragma once

namespace spoLab1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	protected:
	private: System::Windows::Forms::Button^ btnAnalyze;
	private: System::Windows::Forms::Label^ lblMetrics;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->btnAnalyze = (gcnew System::Windows::Forms::Button());
			this->lblMetrics = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(0, 0);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersWidth = 62;
			this->dataGridView1->RowTemplate->Height = 28;
			this->dataGridView1->Size = System::Drawing::Size(1800, 700);
			this->dataGridView1->TabIndex = 0;
			// 
			// btnAnalyze
			// 
			this->btnAnalyze->Location = System::Drawing::Point(12, 790);
			this->btnAnalyze->Name = L"btnAnalyze";
			this->btnAnalyze->Size = System::Drawing::Size(112, 40);
			this->btnAnalyze->TabIndex = 1;
			this->btnAnalyze->Text = L"јнализ";
			this->btnAnalyze->UseVisualStyleBackColor = true;
			this->btnAnalyze->Click += gcnew System::EventHandler(this, &MyForm::btnAnalyze_Click);
			// 
			// lblMetrics
			// 
			this->lblMetrics->AutoSize = true;
			this->lblMetrics->Location = System::Drawing::Point(12, 709);
			this->lblMetrics->Name = L"lblMetrics";
			this->lblMetrics->Size = System::Drawing::Size(0, 20);
			this->lblMetrics->TabIndex = 2;
			this->lblMetrics->Click += gcnew System::EventHandler(this, &MyForm::lblMetrics_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1624, 842);
			this->Controls->Add(this->lblMetrics);
			this->Controls->Add(this->btnAnalyze);
			this->Controls->Add(this->dataGridView1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnAnalyze_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void lblMetrics_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
