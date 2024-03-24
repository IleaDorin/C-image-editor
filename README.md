## Overview

This CLI (Command-Line Interface) program allows users to perform various
image processing operations on PGM (grayscale) and PPM (color) images.

## Features

- Load P2, P3 (ASCII), P5, P6 (Binary) PGM/PPM images.
- Apply filters like EDGE, SHARPEN, BLUR, and GAUSSIAN_BLUR.
- Crop images to a specified area.
- Equalize histograms for enhanced contrast.
- Save modified images in ASCII or Binary formats.

## Commands

- LOAD <filename>: Loads an image.
- SAVE <filename> [ascii]: Saves the current image.
- CROP: Crops the image to the selected area.
- APPLY <filter>: Applies a filter to the image.
- HISTOGRAM <x> <y>: Displays the image histogram.
- EXIT: Exits the program.
- EQUALIZE: Enhances the contrast of the grayscale image.

## Usage

Run the program and enter commands. Ensure the commands and
parameters follow the specified format.

## Note

For the APPLY command, the image must be a color image. If an operation
cannot be performed, the program will display an appropriate message.

## Data Structures Overview

### `grayscale_t`

- **width**: Width of the image in pixels.
- **height**: Height of the image in pixels.
- **max_value**: Maximum grayscale value (usually 255).
- **image**: 2D array of pixel intensity values.
- **selected**: Area within the image selected for operations.

### `color_t`

- **width**: Width of the image in pixels.
- **height**: Height of the image in pixels.
- **max_value**: Maximum color value (usually 255).
- **image**: 2D array of `rgb` structures for pixel colors.
- **selected**: Area within the image selected for operations.

### `selected_t`

- **x1, y1**: Coordinates of the top-left corner of the selected area.
- **x2, y2**: Coordinates of the bottom-right corner.

### `rgb`

- **red**: Red component of a color pixel.
- **green**: Green component of a color pixel.
- **blue**: Blue component of a color pixel.

## Image Loading Process

### Steps:

1. **Open File**: Open in text mode for P2, P3 (ASCII) or binary mode for P5, P6 (Binary).
2. **Read Header**: Extract image type, dimensions, and max pixel value.
3. **Allocate Memory**: Based on dimensions, allocate memory for image data.
4. **Read Image Data**:
   - **ASCII Formats (P2, P3)**: Use `fscanf` to read pixel values.
   - **Binary Formats (P5, P6)**:
     - Use a buffer to read binary data using `fread`.
     - Convert buffer data to image structure format.
5. **Close File**: Close the file after reading.

This process ensures images are correctly loaded into memory, ready for processing.

## Complex Functions in `command.c`

#### `apply_kernel_to_pixel`

- **Purpose**: Applies a convolution kernel to a pixel.
- **Operation**:
  - Iterates over the 3x3 pixel block centered on the target pixel.
  - Multiplies each color component (red, green, blue) of neighboring pixels
    by the corresponding kernel value.
  - Sums these products to calculate new color values.
  - Ensures calculations are only done for pixels with complete neighbors.

#### `apply_filter`

- **Purpose**: Applies a selected filter to the color image.
- **Process**:
  - Chooses the appropriate kernel based on the filter type.
  - Applies the kernel to each pixel within the selected region.
  - Creates a new image to store processed pixels, replaces the old image.

#### `clamp`

Ensures pixel values stay within the 0-255 range.

Filters include:

- **EDGE**: Detects edges by highlighting contrasts.
- **SHARPEN**: Emphasizes edges to make details clearer.
- **BLUR**: Reduces noise and detail for a smoother image.
- **GAUSSIAN_BLUR**: Provides a weighted blur for a soft-focus effect.

Only pixels within the selected area are affected.
For more details, consult the comments in the source files.
