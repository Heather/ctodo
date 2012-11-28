/*          todo - Light TODO list
          Copyright (C)  2012  Ash Harley

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA*/

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
    public partial class MainWindow : Window {
        public todo t;
        public MainWindow() {
            InitializeComponent();
            using (t = new todo()) {
                t.n_initdb();
                }
            }

        private void button1_Click(object sender, RoutedEventArgs e) {
            using (t = new todo()) {
                string[] pony = ("There is pony").Split(' ');
                if (t.n_write(pony, pony.Length) == -1) {
                    MessageBox.Show(t.errorMessage);
                    }
                }
            /*
            List<String> todolist = t.n_read(0, 0);
            foreach (string s in todolist) {
                this.textBox1.Text += s;
                }*/
            }
        }
    }
