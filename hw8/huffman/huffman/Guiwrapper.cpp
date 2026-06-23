// Author: B133040048 吳世騰
// Date  : 2025/12/11
/*
Purpose:
   這個程式提供一個簡易的 Windows 圖形化介面，讓使用者能以點選方式執行
    Huffman 壓縮與解壓縮。介面可選取輸入與輸出檔案，並呼叫外部的
    hw8-b133040048.exe 來完成實際的 Huffman 編碼與還原，避免使用者
    需要自行操作命令列參數，提升使用便利性與操作直覺性。
*/

#include <Windows.h>
#include <iostream>
#include <string>
#include <vcclr.h>

#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;
using namespace System::IO;

// 主視窗類別 HuffmanForm
public ref class HuffmanForm : public Form {
private:
    // 介面元件宣告
    Button^ compressButton;
    Button^ decompressButton;
    TextBox^ inputBox;
    TextBox^ outputBox;
    Label^ inputLabel;
    Label^ outputLabel;
    OpenFileDialog^ openFileDialog;
    SaveFileDialog^ saveFileDialog;

public:
    HuffmanForm() {
        // 設定視窗基本屬性
        this->Text = "Huffman Compressor GUI";
        this->Width = 500;
        this->Height = 250;

        // ---- Input File Label ----
        inputLabel = gcnew Label();
        inputLabel->Text = "Input File:";
        inputLabel->Top = 20;
        inputLabel->Left = 10;
        inputLabel->Width = 80;
        this->Controls->Add(inputLabel);

        // ---- Input File TextBox ----
        inputBox = gcnew TextBox();
        inputBox->Top = 20;
        inputBox->Left = 100;
        inputBox->Width = 250;
        this->Controls->Add(inputBox);

        // ---- Browse Input Button ----
        Button^ browseInput = gcnew Button();
        browseInput->Text = "Browse";
        browseInput->Top = 20;
        browseInput->Left = 370;
        // 點擊後觸發 BrowseInput()
        browseInput->Click += gcnew EventHandler(this, &HuffmanForm::BrowseInput);
        this->Controls->Add(browseInput);

        // ---- Output File Label ----
        outputLabel = gcnew Label();
        outputLabel->Text = "Output File:";
        outputLabel->Top = 60;
        outputLabel->Left = 10;
        outputLabel->Width = 80;
        this->Controls->Add(outputLabel);

        // ---- Output File TextBox ----
        outputBox = gcnew TextBox();
        outputBox->Top = 60;
        outputBox->Left = 100;
        outputBox->Width = 250;
        this->Controls->Add(outputBox);

        // ---- Browse Output Button ----
        Button^ browseOutput = gcnew Button();
        browseOutput->Text = "Browse";
        browseOutput->Top = 60;
        browseOutput->Left = 370;
        browseOutput->Click += gcnew EventHandler(this, &HuffmanForm::BrowseOutput);
        this->Controls->Add(browseOutput);

        // ---- Compress Button ----
        compressButton = gcnew Button();
        compressButton->Text = "Compress";
        compressButton->Top = 110;
        compressButton->Left = 100;
        compressButton->Click += gcnew EventHandler(this, &HuffmanForm::CompressFile);
        this->Controls->Add(compressButton);

        // ---- Decompress Button ----
        decompressButton = gcnew Button();
        decompressButton->Text = "Decompress";
        decompressButton->Top = 110;
        decompressButton->Left = 200;
        decompressButton->Click += gcnew EventHandler(this, &HuffmanForm::DecompressFile);
        this->Controls->Add(decompressButton);

        // 打開、儲存檔案對話框
        openFileDialog = gcnew OpenFileDialog();
        saveFileDialog = gcnew SaveFileDialog();
    }

private:
    // ---- 選擇輸入檔案 ----
    void BrowseInput(Object^ sender, EventArgs^ e) {
        if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            inputBox->Text = openFileDialog->FileName; // 顯示選取的檔案路徑
        }
    }

    // ---- 選擇輸出檔案 ----
    void BrowseOutput(Object^ sender, EventArgs^ e) {
        if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            outputBox->Text = saveFileDialog->FileName;
        }
    }

    // ---- 執行壓縮 ----
    void CompressFile(Object^ sender, EventArgs^ e) {
        RunHuffmanExe("-c"); // 呼叫執行檔並傳入壓縮模式
    }

    // ---- 執行解壓縮 ----
    void DecompressFile(Object^ sender, EventArgs^ e) {
        RunHuffmanExe("-u"); // 呼叫執行檔並傳入解壓縮模式
    }

    // ---- 執行 hw8-b133245007.exe ----
    void RunHuffmanExe(String^ mode) {
        String^ inputFile = inputBox->Text;
        String^ outputFile = outputBox->Text;

        // 確認兩欄都有填
        if (inputFile == "" || outputFile == "") {
            MessageBox::Show("Please select both input and output files.", "Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        // 找出 exe 在當前資料夾的位置
        String^ exePath = Path::Combine(Application::StartupPath, "hw8-b133245007.exe");

        // 若沒找到 exe => 顯示錯誤
        if (!File::Exists(exePath)) {
            MessageBox::Show("Cannot find 'hw8-b133245007.exe' in current folder.", "Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        // 設定 Process 啟動資訊
        ProcessStartInfo^ psi = gcnew ProcessStartInfo();
        psi->FileName = exePath;
        // 組合命令列參數：模式 -i input -o output
        psi->Arguments = String::Format("{0} -i \"{1}\" -o \"{2}\"", mode, inputFile, outputFile);
        psi->UseShellExecute = false;         // 不使用 shell
        psi->RedirectStandardOutput = true;   // 重定向 stdout 讓 GUI 抓輸出
        psi->CreateNoWindow = true;           // 不跳出 cmd 視窗

        // 啟動 Huffman 執行檔
        Process^ process = Process::Start(psi);

        // 讀取 Huffman 程式輸出訊息
        String^ output = process->StandardOutput->ReadToEnd();
        process->WaitForExit();

        // 顯示在 GUI message box
        MessageBox::Show(output, "Huffman Output");
    }
};

[STAThreadAttribute]
int main(array<String^>^ args) {
    // 啟用視覺樣式
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // 執行 GUI 主視窗
    Application::Run(gcnew HuffmanForm());
    return 0;
}
