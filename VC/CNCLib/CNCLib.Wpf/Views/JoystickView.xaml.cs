﻿////////////////////////////////////////////////////////
/*
  This file is part of CNCLib - A library for stepper motors.

  Copyright (c) 2013-2016 Herbert Aitenbichler

  CNCLib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  CNCLib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  http://www.gnu.org/licenses/
*/

using Framework.Wpf.ViewModels;
using System;
using System.Windows;

namespace CNCLib.Wpf
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class JoystickView : Window
    {
        public JoystickView()
        {
            InitializeComponent();
            var vm = DataContext as BaseViewModel;
			if (vm.CloseAction == null)
				vm.CloseAction = new Action(() => this.Close());
		}
	}
}