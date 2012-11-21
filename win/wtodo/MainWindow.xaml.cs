using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using nwrap;

namespace wtodo {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        public todo t;
        public MainWindow() {
            InitializeComponent();
            t = new todo();
            }

        private void button1_Click(object sender, RoutedEventArgs e) {
            //ConsoleManager.Show();
            Console.WriteLine("test");
            t.n_help();
            }
        }
    }
