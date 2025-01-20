import numpy as np

def cosine_distance(array1, array2):
    """Calculate the cosine distance between two 128-length arrays of floats.

    Args:
        array1 (list or np.ndarray): First 128-length array of floats.
        array2 (list or np.ndarray): Second 128-length array of floats.

    Returns:
        float: The cosine distance between the two arrays.
    """
    # Ensure the arrays are numpy arrays
    array1 = np.array(array1)
    array2 = np.array(array2)
    
    # Validate that both arrays are of length 128
    if array1.shape[0] != 128 or array2.shape[0] != 128:
        raise ValueError("Both input arrays must be of length 128.")

    # Calculate the cosine similarity
    dot_product = np.dot(array1, array2)
    norm1 = np.linalg.norm(array1)
    norm2 = np.linalg.norm(array2)

    if norm1 == 0 or norm2 == 0:
        raise ValueError("Input arrays must not be zero vectors.")

    cosine_similarity = dot_product / (norm1 * norm2)

    # Cosine distance is defined as 1 - cosine similarity
    cosine_distance = 1 - cosine_similarity

    return cosine_distance

# Example usage
array1 = [11.8,37,28.8,60.4,6,24.5,28.2,65.9,78.8,69.2,7.6,9.8,24.8,42.2,34.3,19.6,6.3,99.3,96.9,60.3,18.3,15.2,46.3,89.7,34.7,61.2,56,26.2,68.4,78.5,57.6,17.7,40.2,99.5,88.6,27.7,4.3,21.5,57.2,81.9,32.6,8.6,84.8,61.7,47.5,11.6,41.8,37,27.6,4.4,20.1,5.1,30.5,90.9,81.9,52.7,33.6,32.5,17.2,41,88.4,5.7,95.8,16.2,61.8,31.8,37.1,5.8,3.6,70.2,38.5,24.2,4.9,97,59,82.5,5.9,51.4,98.1,66.1,86.8,63.5,42.6,73.8,37.7,66.3,32.7,57.9,26.4,67.8,64.9,41.9,30.7,63.8,85.7,87.1,39.7,14.1,57.2,4.6,73.5,47.9,42.5,1,22.9,31.4,59.2,8.1,82.9,37.2,13.5,3.5,32.7,78.3,98.9,46,96.5,13.6,70.9,5.8,15.2,43.3,43.4,76.6,14.6,20.3,21.6,44.5]
array2 = [79.8,54.5,89.3,38.9,61,50.7,28.8,20.5,36.2,19.8,25.7,23.2,87.2,60.2,13,27.4,15.2,83.3,12.8,52.8,33.5,12.9,87.4,96.2,6.9,67.4,63.4,42.3,43,45.9,14.2,39.5,32.3,37.5,93.2,6.8,39.9,64.8,15.8,64.6,99.8,37,50.1,97.3,88.7,9.8,59.4,93,42.9,18.9,46.3,2.4,76.6,14,47.7,42.2,84.2,65.2,28.6,4.7,82.5,48.1,94.4,46.7,13.5,87.1,15.1,74.4,85.5,19.6,84.8,8,81.2,34.2,2.9,17,42,74.4,54.6,85.2,45.7,77.9,56.2,60.7,30.5,49.2,96.8,90.6,55.6,42.6,91.4,68.7,32.9,19.3,35.3,97.9,75.8,70.6,28.4,4.5,59,6,22,42.1,71.3,30.9,67.1,5.1,15.6,73.6,66.8,6.9,76.4,62.8,74.6,25.4,11.4,28.2,41,76.1,43.6,79.4,69.8,55.5,23,32.6,78.1,48.7]
print("Cosine Distance:", cosine_distance(array1, array2))