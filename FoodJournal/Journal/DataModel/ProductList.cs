using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Data.Json;
using Windows.Storage;

using Newtonsoft.Json;

namespace Journal.DataModel
{
    public sealed class ProductList
    {
        private static ProductList _productList = new ProductList();
        private ObservableCollection<Product> _list = new ObservableCollection<Product>();

        public ObservableCollection<Product> List
        {
            get { return _list; }
        }

        public static async Task<IEnumerable<Product>> GetProductListAsync()
        {
            await _productList.GetProductList();

            return _productList._list;
        }

        public static async Task<IEnumerable<Product>> GetProductListAsync(FoodRecordType type)
        {
            await _productList.GetProductList();

            var matches = _productList.List.Where((product) => product.Type.Contains(type));
            return matches;
        }

        public static async Task<Product> GetProductAsync(String name)
        {
            await _productList.GetProductList();

            var matches = _productList.List.Where((product) => product.Name.Equals(name));
            if (matches.Count() == 1) return matches.First();
            return null;

        }

        public static async Task RemoveProductAsync(Product product)
        {
            await _productList.GetProductList();
            Uri dataUri = new Uri("ms-appx:///DataModel/ProductList.json");
            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(dataUri);

            _productList._list.Remove(product);
            String jsonText = JsonConvert.SerializeObject(_productList._list);

            await FileIO.WriteTextAsync(file, jsonText);
        }

        public static async Task AddProductToListAsync(Product newProduct)
        {
            await _productList.GetProductList();
            Uri dataUri = new Uri("ms-appx:///DataModel/ProductList.json");
            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(dataUri);

            _productList._list.Add(newProduct);
            String jsonText = JsonConvert.SerializeObject(_productList._list);

            await FileIO.WriteTextAsync(file, jsonText);
            //await FileIO.AppendTextAsync(file, jsonText);
        }

        private async Task GetProductList()
        {
            Uri dataUri = new Uri("ms-appx:///DataModel/ProductList.json");

            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(dataUri);
            String jsonText = await FileIO.ReadTextAsync(file);
            _list = new ObservableCollection<Product>(JsonConvert.DeserializeObject<List<Product>>(jsonText));
        }
    }

    [JsonConverter(typeof(Newtonsoft.Json.Converters.StringEnumConverter))]
    public enum FoodRecordType { Snack, Breakfast, Lunch, Dinner };

    public class ProductPortion
    {
        public ProductPortion(float calories = 0, 
            float protein = 0, float fat = 0, float carbons = 0)
        {
            //this.Name = name;
            this.Calories = calories;
            this.Protein = protein;
            this.Fat = fat;
            this.Carbons = carbons;
        }

        public float Calories { get; private set; }
        public float Protein { get; private set; }
        public float Fat { get; private set; }
        public float Carbons { get; private set; }
    }

    public class Product
    {
        public Product(String name, FoodRecordType[] type)
        {
            this.Name = name;
            this.Type = type;
            this.Portion = new Dictionary<string, ProductPortion>();
            //this.CurrentPortion = null;
        }

        public String Name { get; set; }
        public FoodRecordType[] Type { get; set; }
        //public ProductPortion CurrentPortion { get; set; }

        public Dictionary<String, ProductPortion> Portion { get; set; }

        public override string ToString()
        {
            return this.Name;
        }
    }

    public struct ProductPortionView
    {
        public ProductPortionView(String caloriesText, String proteinsText, String fatsText, String carbonsText)
        {
            this.Calories = caloriesText;
            this.Proteins = proteinsText;
            this.Fats = fatsText;
            this.Carbons = carbonsText;
        }

        public String Calories { get; set; }
        public String Proteins { get; set; }
        public String Fats { get; set; }
        public String Carbons { get; set; }
    }
}
