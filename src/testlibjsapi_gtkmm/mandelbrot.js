var $ = function(id) { return builder.getWidget(id); }

builder.addFromFile('mandelbrot.glade');

$('window2').show();

var window = $('window1').show(); 

var area = $('drawingarea1');

var img = area.createImageSurface(window.getWidth(), 1);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// The code below is derived in the main from mandelbrot-set.com.
// The original license is left intact however this is not a verbatim
// copy and paste, some sections have been modified to better fit the
// GTKMM model

/*
 * The Mandelbrot Set, in HTML5 canvas and javascript.
 * https://github.com/cslarsen/mandelbrot-js
 *
 * Copyright (C) 2012 Christian Stigen Larsen
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.  You may obtain
 * a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

/*
 * Global variables:
 */
var zoomStart = 3.4;
var zoom = [zoomStart, zoomStart];
var lookAtDefault = [-0.6, 0];
var lookAt = lookAtDefault;
var xRange = [0, 0];
var yRange = [0, 0];
var escapeRadius = 10.0;
var interiorColor = [0, 0, 0, 255];
var colors = [[0,0,0,0]];

function getColorPicker()
{
  var p = $("colorScheme").value;
  if ( p == "pickColorHSV1" ) return pickColorHSV1;
  if ( p == "pickColorHSV2" ) return pickColorHSV2;
  if ( p == "pickColorHSV3" ) return pickColorHSV3;
  if ( p == "pickColorGrayscale2" ) return pickColorGrayscale2;
  return pickColorGrayscale;
}

function getSamples()
{
  var i = parseInt($('superSamples').value, 10);
  return i<=0? 1 : i;
}

/*
 * Main renderer equation.
 *
 * Returns number of iterations and values of Z_{n}^2 = Tr + Ti at the time
 * we either converged (n == iterations) or diverged.  We use these to
 * determined the color at the current pixel.
 *
 * The Mandelbrot set is rendered taking
 *
 *     Z_{n+1} = Z_{n} + C
 *
 * with C = x + iy, based on the "look at" coordinates.
 *
 * The Julia set can be rendered by taking
 *
 *     Z_{0} = C = x + iy
 *     Z_{n+1} = Z_{n} + K
 *
 * for some arbitrary constant K.  The point C for Z_{0} must be the
 * current pixel we're rendering, but K could be based on the "look at"
 * coordinate, or by letting the user select a point on the screen.
 */
function iterateEquation(Cr, Ci, escapeRadius, iterations)
{
  var Zr = 0;
  var Zi = 0;
  var Tr = 0;
  var Ti = 0;
  var n  = 0;

  for ( ; n<iterations && (Tr+Ti)<=escapeRadius; ++n ) {
    Zi = 2 * Zr * Zi + Ci;
    Zr = Tr - Ti + Cr;
    Tr = Zr * Zr;
    Ti = Zi * Zi;
  }

  /*
   * Four more iterations to decrease error term;
   * see http://linas.org/art-gallery/escape/escape.html
   */
  for ( var e=0; e<4; ++e ) {
    Zi = 2 * Zr * Zi + Ci;
    Zr = Tr - Ti + Cr;
    Tr = Zr * Zr;
    Ti = Zi * Zi;
  }

  return [n, Tr, Ti];
}

/*
 * Return number with metric units
 */
function metric_units(number)
{
  var unit = ["", "k", "M", "G", "T", "P", "E"];
  var mag = Math.ceil((1+Math.log(number)/Math.log(10))/3);
  return "" + (number/Math.pow(10, 3*(mag-1))).toFixed(2) + unit[mag];
}

/*
 * Convert hue-saturation-value/luminosity to RGB.
 *
 * Input ranges:
 *   H =   [0, 360] (integer degrees)
 *   S = [0.0, 1.0] (float)
 *   V = [0.0, 1.0] (float)
 */
function hsv_to_rgb(h, s, v)
{
  if ( v > 1.0 ) v = 1.0;
  var hp = h/60.0;
  var c = v * s;
  var x = c*(1 - Math.abs((hp % 2) - 1));
  var rgb = [0,0,0];

  if ( 0<=hp && hp<1 ) rgb = [c, x, 0];
  if ( 1<=hp && hp<2 ) rgb = [x, c, 0];
  if ( 2<=hp && hp<3 ) rgb = [0, c, x];
  if ( 3<=hp && hp<4 ) rgb = [0, x, c];
  if ( 4<=hp && hp<5 ) rgb = [x, 0, c];
  if ( 5<=hp && hp<6 ) rgb = [c, 0, x];

  var m = v - c;
  rgb[0] += m;
  rgb[1] += m;
  rgb[2] += m;

  rgb[0] *= 255;
  rgb[1] *= 255;
  rgb[2] *= 255;
  return rgb;
}

/*
 * Adjust aspect ratio based on plot ranges and canvas dimensions.
 */
function adjustAspectRatio(xRange, yRange, width, height)
{
  var ratio = Math.abs(xRange[1]-xRange[0]) / Math.abs(yRange[1]-yRange[0]);
  var sratio = width/height;
  if ( sratio>ratio ) {
    var xf = sratio/ratio;
    xRange[0] *= xf;
    xRange[1] *= xf;
      zoom[0] *= xf;
  } else {
    var yf = ratio/sratio;
    yRange[0] *= yf;
    yRange[1] *= yf;
      zoom[1] *= yf;
  }
}

function addRGB(v, w)
{
  v[0] += w[0];
  v[1] += w[1];
  v[2] += w[2];
  v[3] += w[3];
  return v;
}

function divRGB(v, div)
{
  v[0] /= div;
  v[1] /= div;
  v[2] /= div;
  v[3] /= div;
  return v;
}

/*
 * Render the Mandelbrot set
 */
function draw(pickColor, superSamples)
{
  if ( lookAt === null ) lookAt = [-0.6, 0];
  if ( zoom === null ) zoom = [zoomStart, zoomStart];

  xRange = [lookAt[0]-zoom[0]/2, lookAt[0]+zoom[0]/2];
  yRange = [lookAt[1]-zoom[1]/2, lookAt[1]+zoom[1]/2];

  adjustAspectRatio(xRange, yRange, img.width, window.getHeight());

  var steps = parseInt($('steps').value, 10);

  if ( $('autoIterations').checked ) {
    var f = Math.sqrt(
            0.001+2.0 * Math.min(
              Math.abs(xRange[0]-xRange[1]),
              Math.abs(yRange[0]-yRange[1])));

    steps = Math.floor(223.0/f);
    $('steps').value = String(steps);
  }

  var escapeRadius = Math.pow(parseFloat($('escapeRadius').value), 2.0);
  var dx = (xRange[1] - xRange[0]) / (0.5 + (img.width-1));  
  var Ci_step = (yRange[1] - yRange[0]) / (window.height - 0.5);
  
  function drawLineSuperSampled(Ci, off, Cr_init, Cr_step)
  {
    var Cr = Cr_init;

    for ( var x=0, width = img.width; x<width; ++x, Cr += Cr_step ) {
      var color = [0, 0, 0, 255];

      for ( var s=0; s<superSamples; ++s ) {
        var rx = Math.random()*Cr_step;
        var ry = Math.random()*Ci_step;
        var p = iterateEquation(Cr - rx/2, Ci - ry/2, escapeRadius, steps);
        color = addRGB(color, pickColor(steps, p[0], p[1], p[2]));
      }

      color = divRGB(color, superSamples);

      img.data[off++] = color[0];
      img.data[off++] = color[1];
      img.data[off++] = color[2];
      img.data[off++] = 255;
    }
  }

  function drawLine(Ci, off, Cr_init, Cr_step)
  {
    var Cr = Cr_init;

    for ( var x=0, width=img.width; x<width; ++x, Cr += Cr_step ) {
      var p = iterateEquation(Cr, Ci, escapeRadius, steps);
      var color = pickColor(steps, p[0], p[1], p[2]);
      img.data[off++] = color[0];
      img.data[off++] = color[1];
      img.data[off++] = color[2];
      img.data[off++] = 255;
    }
  }

  function drawSolidLine(y, color)
  {
    var off = y*img.width;

    for ( var x=0, width=img.width; x<width; ++x ) {
      img.data[off++] = color[0];
      img.data[off++] = color[1];
      img.data[off++] = color[2];
      img.data[off++] = color[3];
    }
  }

  function render()
  {
    var start  = (new Date).getTime();
    var pixels = 0;
    var Ci = yRange[0];
    var sy = 0;
    var drawLineFunc = superSamples>1? drawLineSuperSampled : drawLine;
    
    for (var sy = 0, height = window.getHeight(); sy < height; ++sy) {
        drawLineFunc(Ci, 0, xRange[0], dx);
        Ci += Ci_step;
        pixels += img.width;
        area.putImageData(img, 0, sy);
    }
    
    // Update speed and time taken
    var now = (new Date).getTime();
    var elapsedMS = now - start;
    $('renderTime').value = (elapsedMS/1000.0).toFixed(1); // 1 comma
    
    var speed = Math.floor(pixels / elapsedMS);

    if ( metric_units(speed).substr(0,3)=="NaN" ) {
      speed = Math.floor(60.0*pixels / elapsedMS);
      $('renderSpeedUnit').value = 'minute';
    } else
      $('renderSpeedUnit').value = 'second';

    $('renderSpeed').value = metric_units(speed);
  }

  render();
}

// Some constants used with smoothColor
var logBase = 1.0 / Math.log(2.0);
var logHalfBase = Math.log(0.5)*logBase;

function smoothColor(steps, n, Tr, Ti)
{
  /*
   * Original smoothing equation is
   *
   * var v = 1 + n - Math.log(Math.log(Math.sqrt(Zr*Zr+Zi*Zi)))/Math.log(2.0);
   *
   * but can be simplified using some elementary logarithm rules to
   */
  return 5 + n - logHalfBase - Math.log(Math.log(Tr+Ti))*logBase;
}

function pickColorHSV1(steps, n, Tr, Ti)
{
  if ( n == steps ) // converged?
    return interiorColor;

  var v = smoothColor(steps, n, Tr, Ti);
  var c = hsv_to_rgb(360.0*v/steps, 1.0, 1.0);
  c.push(255); // alpha
  //return c;
  return rgba_to_bgra(c);
}

function pickColorHSV2(steps, n, Tr, Ti)
{
  if ( n == steps ) // converged?
    return interiorColor;

  var v = smoothColor(steps, n, Tr, Ti);
  var c = hsv_to_rgb(360.0*v/steps, 1.0, 10.0*v/steps);
  c.push(255); // alpha
  //return c;  
  return rgba_to_bgra(c);
}

function pickColorGrayscale(steps, n, Tr, Ti)
{
  if ( n == steps ) // converged?
    return interiorColor;

  var v = smoothColor(steps, n, Tr, Ti);
  v = Math.floor(512.0*v/steps);
  if ( v > 255 ) v = 255;
  return [v, v, v, 255];
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

function rgba_to_bgra(arr) {
    var temp = arr[0];
    arr[0] = Math.floor(arr[2]);
    arr[1] = Math.floor(arr[1]);
    arr[2] = Math.floor(temp);
    return arr;
}

builder.getWidget('submitButton').onClick(function() {
    draw(getColorPicker(), getSamples());
});

app.run(window);
