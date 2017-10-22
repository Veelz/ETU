using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;
using Newtonsoft.Json;
using Microsoft.Live;

namespace Journal.DataModel
{
    public class FoodRecord
    {
        public FoodRecord(String uniqueId, FoodRecordType type, DateTime date)
        {
            this.UniqueId = uniqueId;
            this.Type = type;
            this.Date = date;
            this.Products = new Dictionary<String, ProductPortion>();
        }

        public String UniqueId { get; private set; }
        public FoodRecordType Type { get; private set; }
        public DateTime Date { get; private set; }
        public Dictionary<String, ProductPortion> Products { get; private set; }

        public override string ToString()
        {
            return UniqueId;
        }
    }

    public sealed class FoodRecordsList
    {
        private static FoodRecordsList _FoodRecordList = new FoodRecordsList();
        private ObservableCollection<FoodRecord> _list = new ObservableCollection<FoodRecord>();
        private Dictionary<DateTime, float> _caloriesDict = new Dictionary<DateTime, float>();
        private Dictionary<DateTime, float> _carbonsDict = new Dictionary<DateTime, float>();
        private Dictionary<DateTime, float> _fatsDict = new Dictionary<DateTime, float>();
        private Dictionary<DateTime, float> _proteinsDict = new Dictionary<DateTime, float>();
        private Uri dataUri = new Uri("ms-appx:///DataModel/FoodRecords.json");

        public ObservableCollection<FoodRecord> List
        {
            get { return this._list; }
        }

        public Dictionary<DateTime, float> CaloriesDictionary
        {
            get { return this._caloriesDict; }
        }

        public Dictionary<DateTime, float> CarbonsDictionary
        {
            get { return this._carbonsDict; }
        }

        public Dictionary<DateTime, float> FatsDictionary
        {
            get { return this._fatsDict; }
        }

        public Dictionary<DateTime, float> ProteinsDictionary
        {
            get { return this._proteinsDict; }
        }

        /// <summary>
        /// Return sum of calories at the date
        /// </summary>
        /// <param name="date">Date</param>
        /// <returns>sum of calories at the date</returns>
        public static async Task<float> GetCaloryAtDay(DateTime date)
        {
            await _FoodRecordList.GetRecordList();
            float totalCalory = 0f;
            var foodRecords = _FoodRecordList.List.Where((record) => record.Date.Date.Equals(date.Date));
            foreach (var foodRecord in foodRecords)
            {
                foreach (var product in foodRecord.Products)
                {
                    totalCalory += product.Value.Calories;
                }
            }
            return totalCalory;
        }

        /// <summary>
        /// Return key-value pairs of calories at period including start and end
        /// </summary>
        /// <param name="start">Date</param>
        /// <param name="end">Date</param>
        /// <returns>key-value pairs of calories at period</returns>
        public static async Task<Dictionary<DateTime, float>> GetCaloryAtPeriod(DateTime start, DateTime end)
        {
            if (start > end)
            {
                throw new Exception("Start date can't be later end date");
            }

            await _FoodRecordList.GetRecordList();
            _FoodRecordList.CaloriesDictionary.Clear();

            var foodRecords = _FoodRecordList.List.Where((record) => 
                    (record.Date.Date >= (start))&&(record.Date.Date <= (end)));
            if (foodRecords.Count() != 0)
            {
                foreach (var foodRecord in foodRecords)
                {
                    float calory = 0f;
                    foreach (var product in foodRecord.Products)
                    {
                        calory += product.Value.Calories;
                    }
                    _FoodRecordList._caloriesDict.Add(foodRecord.Date, calory);
                }
            }
            return _FoodRecordList._caloriesDict;
        }

        public static async Task<Dictionary<DateTime, float>> GetProteinAtPeriod(DateTime start, DateTime end)
        {
            if (start > end)
            {
                throw new Exception("Start date can't be later end date");
            }

            await _FoodRecordList.GetRecordList();
            _FoodRecordList.ProteinsDictionary.Clear();

            var foodRecords = _FoodRecordList.List.Where((record) =>
                    (record.Date.Date >= (start)) && (record.Date.Date <= (end)));
            if (foodRecords.Count() != 0)
            {
                foreach (var foodRecord in foodRecords)
                {
                    float proteins = 0f;
                    foreach (var product in foodRecord.Products)
                    {
                        proteins += product.Value.Protein;
                    }
                    _FoodRecordList._proteinsDict.Add(foodRecord.Date, proteins);
                }
            }
            return _FoodRecordList._proteinsDict;
        }

        public static async Task<Dictionary<DateTime, float>> GetFatsAtPeriod(DateTime start, DateTime end)
        {
            if (start > end)
            {
                throw new Exception("Start date can't be later end date");
            }

            await _FoodRecordList.GetRecordList();
            _FoodRecordList.FatsDictionary.Clear();

            var foodRecords = _FoodRecordList.List.Where((record) =>
                    (record.Date.Date >= (start)) && (record.Date.Date <= (end)));
            if (foodRecords.Count() != 0)
            {
                foreach (var foodRecord in foodRecords)
                {
                    float fats = 0f;
                    foreach (var product in foodRecord.Products)
                    {
                        fats += product.Value.Fat;
                    }
                    _FoodRecordList._fatsDict.Add(foodRecord.Date, fats);
                }
            }
            return _FoodRecordList._fatsDict;
        }

        public static async Task<Dictionary<DateTime, float>> GetCarbonsAtPeriod(DateTime start, DateTime end)
        {
            if (start > end)
            {
                throw new Exception("Start date can't be later end date");
            }

            await _FoodRecordList.GetRecordList();
            _FoodRecordList.CarbonsDictionary.Clear();

            var foodRecords = _FoodRecordList.List.Where((record) =>
                    (record.Date.Date >= (start)) && (record.Date.Date <= (end)));
            if (foodRecords.Count() != 0)
            {
                foreach (var foodRecord in foodRecords)
                {
                    float carbons = 0f;
                    foreach (var product in foodRecord.Products)
                    {
                        carbons += product.Value.Carbons;
                    }
                    _FoodRecordList._carbonsDict.Add(foodRecord.Date, carbons);
                }
            }
            return _FoodRecordList._carbonsDict;
        }

        public static async Task<IEnumerable<FoodRecord>> GetFoodRecordsAsync()
        {
            await _FoodRecordList.GetRecordList();

            return _FoodRecordList._list;
        }

        public static async Task<FoodRecord> GetFoodRecordAsync(String uniqueId)
        {
            await _FoodRecordList.GetRecordList();
            var matches = _FoodRecordList.List.Where((foodRecord) => foodRecord.UniqueId.Equals(uniqueId));
            if (matches.Count() == 1) return matches.First();
            return null;
        }

        public static async Task<String> GetProductAsync(String name)
        {
            await _FoodRecordList.GetRecordList();
            var matches = _FoodRecordList.List.SelectMany(list => list.Products).Where((product) => product.Key.Equals(name));
            if (matches.Count() == 1) return matches.First().Key;
            return null;
        }

        public static async Task AddRecordAsync(FoodRecord newRecord)
        {
            //Uri dataUri = new Uri("ms-appx:///DataModel/FoodRecords.json");
            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(_FoodRecordList.dataUri);
            _FoodRecordList.List.Add(newRecord);
            String jsonText = JsonConvert.SerializeObject(_FoodRecordList._list, Formatting.Indented);
            await FileIO.WriteTextAsync(file, jsonText);
        }

       private async Task GetRecordList()
        {
            //Uri dataUri = new Uri("ms-appx:///DataModel/FoodRecords.json");
            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(_FoodRecordList.dataUri);
            String jsonText = await FileIO.ReadTextAsync(file);
            _list = new ObservableCollection<FoodRecord>(JsonConvert.DeserializeObject<List<FoodRecord>>(jsonText));
        }
    }

    public struct FoodRecordCollection
    {
        public FoodRecordCollection(String date, String dayOfWeek, String caloriesText, String uniqueId = "1")
        {
            this.Date = date;
            this.Calories = caloriesText;
            this.DayOfWeek = dayOfWeek;
            this.UniqueId = uniqueId;
        }
        
        public String Date { get; set; }
        public String DayOfWeek { get; set; }
        public String Calories { get; set; }
        public String UniqueId { get; set; }
    }
}
