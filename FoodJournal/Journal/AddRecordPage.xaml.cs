using Journal.Common;
using Journal.DataModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.Resources;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.Display;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace Journal
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AddRecordPage : Page
    {
        private readonly NavigationHelper navigationHelper;
        private ObservableDictionary defaultViewModel = new ObservableDictionary();
        private readonly ResourceLoader resourceLoader = ResourceLoader.GetForCurrentView("Resources");
        private Dictionary<String, object> paramDict;
        private String ParamName;
        private String portionName;
        private Product editedProduct;
        private ProductPortion editedPortion;
        private Product clickedItem;

        public AddRecordPage()
        {
            this.InitializeComponent();

            DisplayInformation.AutoRotationPreferences = DisplayOrientations.Portrait;
            this.NavigationCacheMode = NavigationCacheMode.Required;
            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += NavigationHelper_LoadState;
            this.navigationHelper.SaveState += NavigationHelper_SaveState;
        }

        private void NavigationHelper_SaveState(object sender, SaveStateEventArgs e)
        {
            //this.ParamName = this.pivot.SelectedItem.ToString();
            e.PageState["SelectedItem"] = clickedItem;
            e.PageState["ParamName"] = pivot.SelectedItem.ToString();
        }

        private async void NavigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {
            if (e.PageState != null)
            {
                this.clickedItem = e.PageState["SelectedItem"] as Product;
                this.ParamName = e.PageState["ParamName"] as String;
            }
            if (this.paramDict != null)
            {
                var dict = paramDict;
                if (dict.Keys.Contains("product"))
                {
                    this.editedProduct = dict["product"] as Product;
                    this.portionName = dict["portionName"] as String;
                    this.editedPortion = dict["portion"] as ProductPortion;
                }

                //await ProductList.RemoveProductAsync(clickedItem);

                if (editedProduct.Portion.ContainsKey(portionName))
                {
                    editedProduct.Portion[portionName] = editedPortion;
                }
                else
                {
                    editedProduct.Portion.Add(portionName, editedPortion);
                }
                //await ProductList.AddProductToListAsync(editedProduct);   
            }
            //var productList = await ProductList.GetProductListAsync();
            //this.DefaultViewModel["List"] = foodRecordList;
            //var foodRecordList = await FoodRecordsList.GetFoodRecordsAsync();
            var productList = await ProductList.GetProductListAsync();
            var SnackLst = new ObservableCollection<Product>(productList.Where(item => item.Type.Contains(FoodRecordType.Snack)));
            var breakfastLst = new ObservableCollection<Product>(productList.Where(item => item.Type.Contains(FoodRecordType.Breakfast)));
            var lunchLst = new ObservableCollection<Product>(productList.Where(item => item.Type.Contains(FoodRecordType.Lunch)));
            var dinnerLst = new ObservableCollection<Product>(productList.Where(item => item.Type.Contains(FoodRecordType.Dinner)));
            this.DefaultViewModel["SnackList"] = SnackLst;
            this.DefaultViewModel["BreakfastList"] = breakfastLst;
            this.DefaultViewModel["LunchList"] = lunchLst;
            this.DefaultViewModel["DinnerList"] = dinnerLst;
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
        /// <param name="e">Event data that describes how this page was reached.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedTo(e);
            if (e.Parameter is String)
            {
                this.ParamName = e.Parameter as String;
            }
            else if (e.Parameter is Dictionary<String, object>)
            {
                paramDict = e.Parameter as Dictionary<String, object>;
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedFrom(e);
            this.ParamName = this.pivot.SelectedItem.ToString();
        }

        #endregion

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            //Pivot page to show
            if (this.ParamName == null)
            {
                this.pivot.SelectedItem = this.SnackItem;
            }
            else if (this.ParamName.Equals("SnackItem"))
            {
                //this.ParamName = "Snack";
                this.pivot.SelectedItem = this.SnackItem;
            }
            else if (this.ParamName.Equals("BreakfastItem"))
            {
                //this.ParamName = "Breakfast";
                this.pivot.SelectedItem = this.BreakfastItem;
            }
            else if (this.ParamName.Equals("LunchItem"))
            {
                //this.ParamName = "Lunch";
                this.pivot.SelectedItem = this.LunchItem;
            }
            else if (this.ParamName.Equals("DinnerItem"))
            {
                //this.ParamName = "Dinner";
                this.pivot.SelectedItem = this.DinnerItem;
            }
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            if (!Frame.Navigate(typeof(ProductParameterPage), null))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }
        }

        private void FindAppBarButton_Click(object sender, RoutedEventArgs e)
        {

        }
        /// <summary>
        /// Adds selected item to SelectedSnackItem, deletes selected item from SnackList
        /// </summary>
        private async void SnackList_ItemClick(object sender, ItemClickEventArgs e)
        {
            var item = e.ClickedItem;
            this.SelectedSnackList.Items.Add(item);
            var list = DefaultViewModel["SnackList"] as ObservableCollection<Product>;
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High,
                () => list.Remove(item as Product));
        }

        /// <summary>
        /// Transists to Product's Parameter page 
        /// </summary>
        private void SelectedList_ItemClick(object sender, ItemClickEventArgs e)
        {
            var itemId = e.ClickedItem;
            clickedItem = itemId as Product;
            if (!Frame.Navigate(typeof(ProductParameterPage), itemId))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }
        }

        private async void BreakfastList_ItemClick(object sender, ItemClickEventArgs e)
        {
            var item = e.ClickedItem;
            this.SelectedBreakfastList.Items.Add(item);
            var list = DefaultViewModel["BreakfastList"] as ObservableCollection<Product>;
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High,
                () => list.Remove(item as Product));
        }

        private async void LunchList_ItemClick(object sender, ItemClickEventArgs e)
        {
            var item = e.ClickedItem;
            this.SelectedLunchList.Items.Add(item);
            var list = DefaultViewModel["LunchList"] as ObservableCollection<Product>;
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High,
                () => list.Remove(item as Product));
        }

        private async void DinnerList_ItemClick(object sender, ItemClickEventArgs e)
        {
            var item = e.ClickedItem;
            this.SelectedDinnerList.Items.Add(item);
            var list = DefaultViewModel["DinnerList"] as ObservableCollection<Product>;
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High,
                () => list.Remove(item as Product));
        }

        private void SelectedSnackList_ContainerContentChanging(ListViewBase sender, ContainerContentChangingEventArgs args)
        {
            //SnackList.Items.Remove(args.Item);
            //var frl = DefaultViewModel["List"] as List<FoodRecord>;
        }

        private async void SaveAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            var date = DateTime.Now;
            FoodRecordType frt = FoodRecordType.Snack;
            ListView lstview = null;
            switch(this.pivot.SelectedIndex)
            {
                case 0:
                    frt = FoodRecordType.Snack;
                    lstview = SelectedSnackList;
                    break;
                case 1:
                    frt = FoodRecordType.Breakfast;
                    lstview = SelectedBreakfastList;
                    break;
                case 2:
                    frt = FoodRecordType.Lunch;
                    lstview = SelectedLunchList;
                    break;
                case 3:
                    frt = FoodRecordType.Dinner;
                    lstview = SelectedDinnerList;
                    break;
            }
            var record = new FoodRecord(date.ToString(), frt, date);
            foreach (Product p in lstview.Items)
            {
                if ((this.portionName != null)&&(p.Portion.ContainsKey(this.portionName)))
                {
                    record.Products.Add(p.Name, p.Portion[this.portionName]);
                }
                else
                {
                    record.Products.Add(p.Name, p.Portion.First().Value);
                }
                //record.Products[] = ;
            }
            await FoodRecordsList.AddRecordAsync(record);

            var stackPage = Frame.BackStack;
            int countBackStackDepth = this.Frame.BackStackDepth;
            var backToPage = stackPage[countBackStackDepth - 1];

            this.Frame.Navigate(typeof(HubPage));

            countBackStackDepth = this.Frame.BackStackDepth;
            stackPage.Remove(stackPage[countBackStackDepth - 1]);
            stackPage.Remove(stackPage[countBackStackDepth - 2]);
        }
    }
}
