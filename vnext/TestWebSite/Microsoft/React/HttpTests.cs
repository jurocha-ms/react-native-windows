namespace Microsoft.React.Test
{
  public sealed class HttpTests
  {
    public static async Task ArbitraryLengthHexText(HttpContext context)
    {
      var response = context.Response;
      string? lenthString = context.Request.RouteValues["Length"]!.ToString();

      int length;
      if (! int.TryParse(lenthString, out length) || length < 1)
      {
        response.StatusCode = 400;
        await response.Body.WriteAsync(new byte[]{});

        return;
      }

      response.StatusCode = 200;
      response.ContentType = "text/plain";
      response.ContentLength = length;

      var alphabet = "0123456789ABCDEF".ToCharArray();
      var buffer = new byte[1024];
      for(int i=0; i < buffer.Length; i++)
      {
        buffer[i] = (byte) alphabet[i % alphabet.Length];
      }

      int blocks = length / buffer.Length;
      for(int i=0; i < blocks; i++)
      {
        await response.Body.WriteAsync(buffer);
      }

      // Remaining bytes (if any)
      await response.Body.WriteAsync(buffer, 0, length % buffer.Length);
    }
  }
}
