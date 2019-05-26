using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
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
        
        private async Task GetAllValuesAsync()
        {
            var pathDataCurrencies = $"../../Files/{Store.OutputFileName}.txt";

            var acceptCurrenciesTask = GetAcceptCurrencies();
            var dataTask = _transport.GetCurrenciesData();
            
            await Task.WhenAll(acceptCurrenciesTask, dataTask);

            var acceptCurrencies = await acceptCurrenciesTask;
            var data = await dataTask;
            var isFilter = acceptCurrencies != null;

            using (var writer = new StreamWriter(pathDataCurrencies))
            {
                IDictionary<string, JToken> currencies = data.Valute;
                foreach (var item in currencies)
                {
                    if (isFilter && !acceptCurrencies.Contains(item.Key)) continue;
                    writer.WriteLineAsync($"{item.Value["Nominal"]} {item.Key} по курсу {item.Value["Value"]}");
                }
            }
        }

        private async Task GetAllValuesSync()
        {
            var pathDataCurrencies = $"../../Files/{Store.OutputFileName}.txt";
            
            var acceptCurrencies = await GetAcceptCurrencies();
            var data = await _transport.GetCurrenciesData();
            var isFilter = acceptCurrencies != null;

            using (var writer = new StreamWriter(pathDataCurrencies))
            {
                IDictionary<string, JToken> currencies = data.Valute;
                foreach (var item in currencies)
                {
                    if (isFilter && !acceptCurrencies.Contains(item.Key)) continue;
                    writer.WriteLine($"{item.Value["Nominal"]} {item.Key} по курсу {item.Value["Value"]}");
                }
            }
        }

        private async Task<List<string>> GetAcceptCurrencies()
        {
            var pathAcceptCurrencies = $"../../Files/{Store.InputFileName}.json";
            if (!File.Exists(pathAcceptCurrencies)) return null;
            using (var reader = new StreamReader(pathAcceptCurrencies))
            {
                var file = await reader.ReadToEndAsync();
                return JsonConvert.DeserializeObject<List<string>>(file);
            }
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
        
        public async void OkButtonMouseClick(object sender, EventArgs e)
        {
            var elapsedMs = new List<long>();
            for (var i = 0; i < Store.CountIteration; i++)
            {
                var watch = Stopwatch.StartNew();

                await GetAllValuesSync();
                
                watch.Stop();
                elapsedMs.Add(watch.ElapsedMilliseconds);
            }
            MessageBox.Show("Work sync: \n" +string.Join("\n", elapsedMs.Select(
                (time, index) => index + 1 + @": Work time: " + time + @"ms"))
            );
            
            elapsedMs = new List<long>();
            for (var i = 0; i < Store.CountIteration; i++)
            {
                var watch = Stopwatch.StartNew();

                await GetAllValuesAsync();
                
                watch.Stop();
                elapsedMs.Add(watch.ElapsedMilliseconds);
            }
            MessageBox.Show("Work async: \n" + string.Join("\n", elapsedMs.Select(
                (time, index) => index + 1 + @": Work time: " + time + @"ms"))
            );
        }
    }
}