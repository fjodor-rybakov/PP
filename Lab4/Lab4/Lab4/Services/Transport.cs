using System;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Lab4.Services
{
    public class Transport
    {
        private static readonly Uri BaseAddress = new Uri("https://www.cbr-xml-daily.ru/");
        private static readonly HttpClient Client = new HttpClient {BaseAddress = BaseAddress};
        
        public async Task<dynamic> GetCurrenciesData()
        {
            var response = await Client.GetAsync("daily_json.js");
            var json = response.StatusCode != HttpStatusCode.OK ? null : await response.Content.ReadAsStringAsync();
            return json == null ? null : JsonConvert.DeserializeObject(json);
        }
    }
}