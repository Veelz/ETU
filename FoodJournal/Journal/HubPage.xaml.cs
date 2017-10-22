using Journal.Common;
using Journal.DataModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.Resources;
using Windows.Data.Xml.Dom;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.Display;
using Windows.UI.Core;
using Windows.UI.Notifications;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;
using Windows.Storage;
using WinRTXamlToolkit.Controls.DataVisualization.Charting;
using Microsoft.Live;


// The Hub Application template is documented at http://go.microsoft.com/fwlink/?LinkId=391641

namespace Journal
{
    /// <summary>
    /// A page that displays a grouped collection of items.
    /// </summary>
    public sealed partial class HubPage : Page
    {
        private readonly NavigationHelper navigationHelper;
        private readonly ObservableDictionary defaultViewModel = new ObservableDictionary();
        private readonly ResourceLoader resourceLoader = ResourceLoader.GetForCurrentView("Resources");
        //private readonly List<FoodRecordCollection> foodRecordCollection = new List<FoodRecordCollection>();

        public HubPage()
        {
            this.InitializeComponent();

            // Hub is only supported in Portrait orientation
            DisplayInformation.AutoRotationPreferences = DisplayOrientations.Portrait;

            this.NavigationCacheMode = NavigationCacheMode.Required;

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
        private async void NavigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {
            var foodRecordList = await FoodRecordsList.GetFoodRecordsAsync();
            this.DefaultViewModel["List"] = foodRecordList;

            var start = DateTime.Today.AddDays(-30);
            var end = DateTime.Today;

            var caloryDict = await FoodRecordsList.GetCaloryAtPeriod(start, end);
            var carbonsDict = await FoodRecordsList.GetCarbonsAtPeriod(start, end);
            var fatsDict = await FoodRecordsList.GetFatsAtPeriod(start, end);
            var proteinDict = await FoodRecordsList.GetProteinAtPeriod(start, end);
            this.defaultViewModel["CaloryDict"] = caloryDict.Skip(caloryDict.Count - 7)
                .ToDictionary(item => item.Key, item => item.Value);
            this.defaultViewModel["CarbonsDict"] = carbonsDict.Skip(carbonsDict.Count - 7)
                .ToDictionary(item => item.Key, item => item.Value);    
            this.defaultViewModel["FatsDict"] = fatsDict.Skip(fatsDict.Count - 7)
                .ToDictionary(item => item.Key, item => item.Value);         
            this.defaultViewModel["ProteinsDict"] = proteinDict.Skip(proteinDict.Count - 7)
                .ToDictionary(item => item.Key, item => item.Value);    

            caloryDict = caloryDict.OrderByDescending(item => item.Key)
                                   .ToDictionary(item => item.Key, item => item.Value);
            List<FoodRecordCollection> foodRecordCollection = new List<FoodRecordCollection>();
            foreach (var key in caloryDict.Keys)
            {
                String caloriesText = caloryDict[key].ToString() + " " + resourceLoader.GetString("CaloriesUnits");
                var frc = new FoodRecordCollection(key.ToString("d"),
                        resourceLoader.GetString(key.DayOfWeek.ToString()),
                        caloriesText);
                foodRecordCollection.Add(frc);
            }
            this.defaultViewModel["Dict"] = foodRecordCollection;

            String preText = resourceLoader.GetString("TodayItem") + " ";
            String calories = "ничего";
            foreach (var record in caloryDict)
            {
                if (record.Key.Date == DateTime.Today)
                {
                    calories = record.Value.ToString() + " " + resourceLoader.GetString("CaloriesUnits");
                }
            }

            var xml = @"<tile>
                    <visual>
                    <binding template=""TileSquareText01"">
                      <text id=""1"">" + preText + @"</text>
                      <text id=""2"">" + calories + @"</text>
                    </binding>  
                    </visual>
                    </tile>";
            var xmlDocument = new XmlDocument();
            xmlDocument.LoadXml(xml);
            var notification = new TileNotification(xmlDocument);
            TileUpdateManager.CreateTileUpdaterForApplication().Update(notification);
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
            // TODO: Save the unique state of the page here.
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
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedFrom(e);
            this.DefaultViewModel.Clear();
        }

        private void SnackItem_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (!Frame.Navigate(typeof(AddRecordPage), "SnackItem"))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }
        }

        private void BreakfastItem_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (!Frame.Navigate(typeof(AddRecordPage), "BreakfastItem"))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }
        }

        private void LunchItem_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (!Frame.Navigate(typeof(AddRecordPage), "LunchItem"))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }
        }

        private void Dinner_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (!Frame.Navigate(typeof(AddRecordPage), "DinnerItem"))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }
        }

        #endregion

        private void ListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            /*
            var item = e.ClickedItem;
            if (!Frame.Navigate(typeof(FoodRecordPage), item))
            {
                throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
            }*/
        }
        /*
        private async void SyncButton_Click(object sender, RoutedEventArgs e)
        {
            
            Uri dataUri = new Uri("ms-appx:///DataModel/FoodRecords.json");
            bool connected = false;
            try
            {


                var authClient = new LiveAuthClient();
                LiveLoginResult result; 
                
                await authClient.LoginAsync(new string[] { "wl.signin", "wl.skydrive" }, result);
                connected = true;
                if (result.Status == LiveConnectSessionStatus.Connected)
                {
                    var connectClient = new LiveConnectClient(result.Session);
                    StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(dataUri);
                    if (file != null)
                    {
                        //LiveOperationResult uploadResult = await connectClient.BackgroundUploadAsync(skyDriveFolder, "LumpyCircle.png", file, OverwriteOption.Rename);
                        var task = await connectClient.CreateBackgroundUploadAsync("me/skydrive",
                            "foodRecord.json", file, OverwriteOption.Rename);
                        await task.StartAsync();
                    }
                    //var meResult = await connectClient.GetAsync("me");
                    //dynamic meData = meResult.Result;
                    //updateUI(meData);
                }
            }
            catch (LiveAuthException ex)
            {
                //return false;
            }
            catch (LiveConnectException ex)
            {
                //return false;
            }
            //return true;
            // Turn off the display of the connection button in the UI.
            this.SyncButton.IsEnabled = !connected;
    }*/
    }
}
