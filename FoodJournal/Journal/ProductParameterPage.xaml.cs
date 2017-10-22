using Journal.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.Display;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Journal.DataModel;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace Journal
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ProductParameterPage : Page
    {
        private NavigationHelper navigationHelper;
        private ObservableDictionary defaultViewModel = new ObservableDictionary();
        private Product clickedProduct;
        private String SelectedRadiobutton;
        private bool isNewItem;

        public ProductParameterPage()
        {
            this.InitializeComponent();

            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += this.NavigationHelper_LoadState;
            this.navigationHelper.SaveState += this.NavigationHelper_SaveState;
        }

        /// <summary>
        /// Gets the <see cref="NavigationHelper"/> associated with this <see cref="Page"/>.
        /// </summary>
        public NavigationHelper NavigationHelper
        {
            get { return this.navigationHelper; }
        }

        /// <summary>
        /// Gets the view model for this <see cref="Page"/>.
        /// This can be changed to a strongly typed view model.
        /// </summary>
        public ObservableDictionary DefaultViewModel
        {
            get { return this.defaultViewModel; }
        }

        /// <summary>
        /// Populates the page with content passed during navigation.  Any saved state is also
        /// provided when recreating a page from a prior session.
        /// </summary>
        /// <param name="sender">
        /// The source of the event; typically <see cref="NavigationHelper"/>
        /// </param>
        /// <param name="e">Event data that provides both the navigation parameter passed to
        /// <see cref="Frame.Navigate(Type, Object)"/> when this page was initially requested and
        /// a dictionary of state preserved by this page during an earlier
        /// session.  The state will be null the first time a page is visited.</param>
        private void NavigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {

        }

        /// <summary>
        /// Preserves state associated with this page in case the application is suspended or the
        /// page is discarded from the navigation cache.  Values must conform to the serialization
        /// requirements of <see cref="SuspensionManager.SessionState"/>.
        /// </summary>
        /// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
        /// <param name="e">Event data that provides an empty dictionary to be populated with
        /// serializable state.</param>
        private void NavigationHelper_SaveState(object sender, SaveStateEventArgs e)
        {
            //var x = await ProductList.GetProductAsync(clickedProduct.Name);
        }

        #region NavigationHelper registration

        /// <summary>
        /// The methods provided in this section are simply used to allow
        /// NavigationHelper to respond to the page's navigation methods.
        /// <para>
        /// Page specific logic should be placed in event handlers for the  
        /// <see cref="NavigationHelper.LoadState"/>
        /// and <see cref="NavigationHelper.SaveState"/>.
        /// The navigation parameter is available in the LoadState method 
        /// in addition to page state preserved during an earlier session.
        /// </para>
        /// </summary>
        /// <param name="e">Provides data for navigation methods and event
        /// handlers that cannot cancel the navigation request.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedTo(e);
            if (e.Parameter != null)
            {
                this.clickedProduct = e.Parameter as Product;
                isNewItem = false;
            }
            else
            {
                FoodRecordType[] frt = { };
                this.clickedProduct = new Product(String.Empty, frt);
                isNewItem = true;
            }

            /*
            this.Name = product.Name;
            this.CaloriesInput.Text = product.Calories.ToString();
            this.CarbonsInput.Text = product.Carbons.ToString();
            this.FatInput.Text = product.Fat.ToString();
            this.ProteinInput.Text = product.Protein.ToString();
            */
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedFrom(e);
        }

        #endregion

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            if (!isNewItem)
            {
                this.productName.Text = this.clickedProduct.Name;
                this.SnackItemCheckbox.IsChecked = clickedProduct.Type.Contains(FoodRecordType.Snack);
                this.BreakfastItemCheckbox.IsChecked = clickedProduct.Type.Contains(FoodRecordType.Breakfast);
                this.LunchItemCheckbox.IsChecked = clickedProduct.Type.Contains(FoodRecordType.Lunch);
                this.DinnerItemCheckbox.IsChecked = clickedProduct.Type.Contains(FoodRecordType.Dinner);
                int i = 0;
                // Добавление RadioButton -ов
                foreach (var p in this.clickedProduct.Portion)
                {
                    RadioButton rb = new RadioButton { IsChecked = false, GroupName = "Portions", Content = p.Key };
                    rb.Checked += RadioButton_Checked;
                    if (i++ == 0)
                    {//facepalm
                        SelectedRadiobutton = p.Key;
                        this.PortionNameInput.Text = p.Key.ToString();
                        this.CaloriesInput.Text = p.Value.Calories.ToString();
                        this.CarbonsInput.Text = p.Value.Carbons.ToString();
                        this.FatInput.Text = p.Value.Fat.ToString();
                        this.ProteinInput.Text = p.Value.Protein.ToString();
                        rb.IsChecked = true;
                    }
                    this.PortionButtons.Children.Add(rb);
                }
            }
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton rb = (RadioButton)sender;
            var p = this.clickedProduct.Portion[(String)rb.Content];
            this.PortionNameInput.Text = (String)rb.Content;
            this.CaloriesInput.Text = p.Calories.ToString();
            this.CarbonsInput.Text = p.Carbons.ToString();
            this.FatInput.Text = p.Fat.ToString();
            this.ProteinInput.Text = p.Protein.ToString();
            SelectedRadiobutton = (String)rb.Content;
        }

        private void SaveAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            //var productList = await ProductList.GetProductListAsync() as List<Product>;
            //productList.Remove(clickedProduct);

            float calories = float.Parse(CaloriesInput.Text);
            float protein = float.Parse(ProteinInput.Text);
            float carbons = float.Parse(CarbonsInput.Text);
            float fats = float.Parse(FatInput.Text);
            String name = productName.Text;
            ProductPortion pp = new ProductPortion(calories, protein, fats, carbons);
            List<FoodRecordType> frt = new List<FoodRecordType>();
            if (this.SnackItemCheckbox.IsChecked.Value)
            {
                frt.Add(FoodRecordType.Snack);
            }
            if (this.BreakfastItemCheckbox.IsChecked.Value)
            {
                frt.Add(FoodRecordType.Breakfast);
            }
            if (this.LunchItemCheckbox.IsChecked.Value)
            {
                frt.Add(FoodRecordType.Lunch);
            }
            if (this.DinnerItemCheckbox.IsChecked.Value)
            {
                frt.Add(FoodRecordType.Dinner);
            }
            clickedProduct.Name = name;
            clickedProduct.Type = frt.ToArray();
            clickedProduct.Portion[this.SelectedRadiobutton] = pp;

            String key;
            if (isNewItem)
            {
                key = "newproduct";
            } 
            else
            {
                key = "product";
            }
            var param = new Dictionary<string, object>
            {
                [key] = clickedProduct,
                ["portionName"] = SelectedRadiobutton,
                ["portion"] = pp
            };
            /*GoBack with parameter*/
            var stackPage = Frame.BackStack;
            int countBackStackDepth = this.Frame.BackStackDepth;
            var backToPage = stackPage[countBackStackDepth - 1];

            this.Frame.Navigate(typeof(AddRecordPage), param);

            countBackStackDepth = this.Frame.BackStackDepth;
            stackPage.Remove(stackPage[countBackStackDepth - 1]);
            stackPage.Remove(stackPage[countBackStackDepth - 2]);

            //clickedProduct = new Product(name, frt.ToArray());
            //clickedProduct.Portion.Add(this.SelectedRadiobutton, pp);

            //productList.Add(clickedProduct);
            //if (!Frame.Navigate(typeof(AddRecordPage), clickedProduct))
            //{
            //    throw new Exception("NavigationFailedExceptionMessage");
            //}
        }
    }
}
