using System.Windows.Forms;

namespace Lab4
{
    public partial class View : InputForm
    {
        private readonly ViewController _viewController = new ViewController();
        
        public View()
        {
            InitializeComponent();
            CreateInputs();
            CreateButton();
        }
        
        private void CreateInputs()
        {
            Store.InputFileName = "accept_currencies";
            Store.OutputFileName = "data_currencies";
            Store.CountIteration = 30;

            var inputFileNameBox = AddTextBox("On current directory\n" +
                                              "Input file name: ", Store.InputFileName);
            var outputFileNameBox = AddTextBox("Output file name: ", Store.OutputFileName);
            var countIterationBox = AddTextBox("Count iteration: ", Store.CountIteration.ToString());

            inputFileNameBox.KeyUp += _viewController.OnInputFileName;
            outputFileNameBox.KeyUp += _viewController.OnOutputFileName;
            countIterationBox.KeyUp += _viewController.OnChangeCountIteration;
        }

        private void CreateButton()
        {
            var okButton = new Button {Text = @"Get result"};
            okButton.MouseClick += _viewController.OkButtonMouseClick;
            StartPosition = FormStartPosition.CenterScreen;
            Panel.Controls.Add(okButton);
        }
    }
}