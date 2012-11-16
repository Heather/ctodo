/*          ctodo - Light TODO library
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

//#import('packages/sqlite/sqlite.dart');
#import('dart:html');
main() {
	query('#status').text = 'Hi, Dart';
	print('test');
	//window.alert('Confirmed!');
	/*var c = new sqlite.Database("todo.db3");
	try {
		// Simple queries and statements
		Row results = c.first("SELECT ?+2, UPPER(?)", [3, "Hello"]);
		var five = results[0];
		var shouting = results[1];

		// Iterating over a result set
		var count = c.execute("SELECT * FROM posts LIMIT 10", callback: (row) {
			print("${row.title}: ${row.body}");
			});
		print("Showing ${count} posts.");

		// Reusing prepared statements
		var statement = c.prepare("INSERT INTO posts (title, body) VALUES (?,?)");
		try {
			statement.execute(["Hi", "Hello world"]);
			statement.execute(["Byte", "Goodbye cruel world"]);
			} 
		finally {
			statement.close();
			}
		} 
	finally {
		c.close();
		}*/
	}