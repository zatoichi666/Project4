#pragma once
/////////////////////////////////////////////////////////////////////
// Form1.h - Demo of techniques to build C++ WinForm interface     //
//                                                                 //
// Jim Fawcett, CSE775 - Distributed Objects, Spring 2006          //
/////////////////////////////////////////////////////////////////////
/*
 * Note:
 * - To build this application you must build the MockChannel DLL first.
 * - You do that by right-clicking on the MockChannel project and selecting
 *   Rebuild.
 */
#using <../Debug/MockChannel.dll>

///////////////////////////////////////////////////////////
// Can't use the include below because CLR builds use type
// information from the dll metadata.  Including causes
// multiple definitions.
//#include "../MockChannel/MockChannel.h"

namespace mockChannel {
  
  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;
  using namespace System::IO;
  using namespace System::Threading;

  /// WARNING:
  /// If you change the name of this class, you will need to change the
  /// 'Resource File Name' property for the managed resource compiler tool
  /// associated with all .resx files this class depends on.  Otherwise,
  /// the designers will not be able to interact properly with localized
  /// resources associated with this form.

  public ref class Form1 : public System::Windows::Forms::Form
  {
  public:
    Form1(void)
    {
      InitializeComponent();
    }

  protected:
    ~Form1()
    {
      if (components)
      {
        delete components;
      }
    }
  protected: 
  private: 
    System::Windows::Forms::Button^  button1;
    System::Windows::Forms::TextBox^  textBox1;
    System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
    System::Windows::Forms::ListBox^  listBox1;
    System::Windows::Forms::Button^  button2;
    System::Windows::Forms::OpenFileDialog^  openFileDialog1;
    System::Windows::Forms::Button^  button3;

    mockChannel::IChannel^ chan;
    delegate void itemDelegate(String^ str);
    itemDelegate^ myItemDelegate;
    System::Windows::Forms::Label^  label1;

  private:
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->textBox1 = (gcnew System::Windows::Forms::TextBox());
      this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
      this->listBox1 = (gcnew System::Windows::Forms::ListBox());
      this->button2 = (gcnew System::Windows::Forms::Button());
      this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
      this->button3 = (gcnew System::Windows::Forms::Button());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // button1
      // 
      this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->button1->Location = System::Drawing::Point(708, 59);
      this->button1->Margin = System::Windows::Forms::Padding(4);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(125, 28);
      this->button1->TabIndex = 0;
      this->button1->Text = L"Browse";
      this->button1->UseVisualStyleBackColor = true;
      this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
      // 
      // textBox1
      // 
      this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBox1->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->textBox1->Location = System::Drawing::Point(68, 59);
      this->textBox1->Margin = System::Windows::Forms::Padding(4);
      this->textBox1->Name = L"textBox1";
      this->textBox1->Size = System::Drawing::Size(612, 27);
      this->textBox1->TabIndex = 1;
      // 
      // listBox1
      // 
      this->listBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBox1->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->listBox1->FormattingEnabled = true;
      this->listBox1->ItemHeight = 19;
      this->listBox1->Location = System::Drawing::Point(68, 111);
      this->listBox1->Margin = System::Windows::Forms::Padding(4);
      this->listBox1->Name = L"listBox1";
      this->listBox1->Size = System::Drawing::Size(612, 327);
      this->listBox1->TabIndex = 2;
      // 
      // button2
      // 
      this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->button2->Location = System::Drawing::Point(708, 111);
      this->button2->Margin = System::Windows::Forms::Padding(4);
      this->button2->Name = L"button2";
      this->button2->Size = System::Drawing::Size(125, 28);
      this->button2->TabIndex = 0;
      this->button2->Text = L"Get Files";
      this->button2->UseVisualStyleBackColor = true;
      this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
      // 
      // openFileDialog1
      // 
      this->openFileDialog1->FileName = L"openFileDialog1";
      // 
      // button3
      // 
      this->button3->Location = System::Drawing::Point(708, 170);
      this->button3->Name = L"button3";
      this->button3->Size = System::Drawing::Size(125, 32);
      this->button3->TabIndex = 3;
      this->button3->Text = L"PostMessage";
      this->button3->UseVisualStyleBackColor = true;
      this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label1->Location = System::Drawing::Point(68, 23);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(220, 20);
      this->label1->TabIndex = 4;
      this->label1->Text = L"C++\\CLI Client Prototype";
      // 
      // Form1
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(881, 490);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->button3);
      this->Controls->Add(this->listBox1);
      this->Controls->Add(this->textBox1);
      this->Controls->Add(this->button2);
      this->Controls->Add(this->button1);
      this->Margin = System::Windows::Forms::Padding(4);
      this->Name = L"Form1";
      this->Text = L"Form1";
      this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
  //----< receive thread binds delegate to this to return its result >---

  private:
    void AddItem(String^ item)
    {
      String^ msg = "Received Message: " + item;
      listBox1->Items->Add(msg);
    }
  //----< Receive thread function gets message and displays it >---------

  private:
    void Run()
    {
      while(true)
      {
        String^ msg = chan->getMessage();
        if(this->InvokeRequired)
          this->Invoke(myItemDelegate, gcnew array<System::Object^, 1> { msg });
        else
          this->AddItem(msg);
      }
    }
  //----< Initialize Form on startup >-----------------------------------

  private: 
    System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
    {
      label1->Text = "C++\\CLI Client Prototype";
      button3->Enabled = false;
      this->Text = "C++\\CLI WinForm Demonstraton";
      textBox1->Text = Directory::GetCurrentDirectory();
      array<String^>^ files = Directory::GetFiles(textBox1->Text, "*.*");
      for(int i=0; i<files->Length; ++i)
      {
        String^ name = Path::GetFileName(files[i]);
        listBox1->Items->Add(name);
      }

      ///////////////////////////////////////////////////////////////////
      // Register function for thread to return its information
      myItemDelegate += gcnew itemDelegate(this, &Form1::AddItem);

      ///////////////////////////////////////////////////////////////////
      // create instance of MockChannel to echo back messages
      chan = mockChannel::IChannel::CreateChannel();

      ///////////////////////////////////////////////////////////////////
      // This is a .Net thread.  Its purpose is to handle received
      // messages.  If IsBackground is false process won't shut down
      // until thread terminates.
      Thread^ clientReceiveThread = gcnew Thread(gcnew ThreadStart(this, &Form1::Run));
      clientReceiveThread->IsBackground = true;
      clientReceiveThread->Start();
    }
  //----< Show folder browser and get user's selected path >-------------

  private: 
    System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
    {
      folderBrowserDialog1 = gcnew FolderBrowserDialog();
      folderBrowserDialog1->ShowNewFolderButton = false;
      folderBrowserDialog1->SelectedPath = Directory::GetCurrentDirectory();
      System::Windows::Forms::DialogResult result;
      result = folderBrowserDialog1->ShowDialog();
      if(result == System::Windows::Forms::DialogResult::OK)
      {
        Directory::SetCurrentDirectory(folderBrowserDialog1->SelectedPath);
        textBox1->Text = Directory::GetCurrentDirectory();
        listBox1->Items->Clear();
        array<String^>^ files = Directory::GetFiles(textBox1->Text, "*.*");
        for(int i=0; i<files->Length; ++i)
        {
          String^ name = Path::GetFileName(files[i]);
          listBox1->Items->Add(name);
        }
      }
    }
  //----< Display all files selected by user in file dialog >------------

  private: 
    System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
    {
      openFileDialog1 = gcnew OpenFileDialog();
      openFileDialog1->InitialDirectory = textBox1->Text;
      openFileDialog1->Multiselect = true;
      System::Windows::Forms::DialogResult result = openFileDialog1->ShowDialog();
      if(result == System::Windows::Forms::DialogResult::OK)
      {
        listBox1->Items->Clear();
        array<String^>^ files = openFileDialog1->FileNames;
        int len = files->Length;
        for(int i=0; i<files->Length; ++i)
        {
          String^ name = Path::GetFileName(files[i]);
          listBox1->Items->Add(name);
        }
      }
      button3->Enabled = true;
    }
  //----< post messages from ListBox to MockChannel >--------------------
     
  private: 
    System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
    {
      int numItems = listBox1->Items->Count;
      for(int i=0; i<numItems; ++i)
      {
        String^ msg = (String^)listBox1->Items[i];
        chan->postMessage(msg);
        listBox1->Items->Add("   Posted Message: " + listBox1->Items[i]);
      }
      button3->Enabled = false;
    }
  };
}

