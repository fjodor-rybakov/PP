using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;
using Lab4.Services;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Lab4
{
    public class ViewController
    {
        private readonly Transport _transport = new Transport();
        
        private async void GetAllValues()
        {
            var pathDataCurrencies = $"../../Files/{Store.OutputFileName}.txt";

            var acceptCurrencies = GetAcceptCurrencies();
            var data = await _transport.GetCurrenciesData();
            var isFilter = acceptCurrencies != null;

            using (var writer = new StreamWriter(pathDataCurrencies))
            {
                IDictionary<string, JToken> currencies = data.Valute;
                foreach (var item in currencies)
                {
                    if (isFilter && !acceptCurrencies.Contains(item.Key)) continue;
                    await writer.WriteLineAsync($"{item.Value["Nominal"]} {item.Key} по курсу {item.Value["Value"]}");
                }
            }

            MessageBox.Show(@"Calculated!");
        }

        private List<string> GetAcceptCurrencies()
        {
            var pathAcceptCurrencies = $"../../Files/{Store.InputFileName}.json";
            return !File.Exists(pathAcceptCurrencies) ? null : 
                JsonConvert.DeserializeObject<List<string>>(File.ReadAllText(pathAcceptCurrencies));
        }
        
        public void OnChangeCountIteration(object sender, KeyEventArgs e)
        {
            var value = (TextBox) sender;
            if (value.TextLength == 0)
            {
                Store.CountIteration = 0;
                return;
            }
            if (int.TryParse(value.Text, out var count)) Store.CountIteration = count;
        }
        
        public void OnInputFileName(object sender, KeyEventArgs e)
        {
            var value = (TextBox) sender;
            Store.InputFileName = value.Text;
        }
        
        public void OnOutputFileName(object sender, KeyEventArgs e)
        {
            var value = (TextBox) sender;
            Store.OutputFileName = value.Text;
        }
        
        public void OkButtonMouseClick(object sender, System.EventArgs e)
        {
            GetAllValues();
        }
    }
}