var builder = WebApplication.CreateBuilder(args);

// CORS middleware
//TODO: Subordinate to specific set of tests
var originPolicyName = "AllowedOrigins";
builder.Services.AddCors(
  options =>
  {
    options.AddPolicy(
      name: originPolicyName,
      policy =>
      {
        policy
          .WithOrigins("http://orig.in")
          .WithMethods("GET")
          ;
      });
  });

var app = builder.Build();
app.UseWebSockets();
app.UseCors();
app.UseStaticFiles();

// See https://github.com/dotnet/aspnetcore/blob/v7.0.15/src/Http/Routing/src/RequestDelegateRouteBuilderExtensions.cs
// app.Map("/", () => "Sample HTTP Response");
async Task DefaultRequestDelegate(HttpContext context)
{
  var response = context.Response;
  response.StatusCode = 200;
  response.ContentType = "text/plain";

  var bytes = System.Text.Encoding.UTF8.GetBytes("Sample HTTP Response");
  await response.Body.WriteAsync(bytes);
}

#region Request Mappings

app.Map("/", async context =>
{
  if (context.WebSockets.IsWebSocketRequest)
  {
    // ws://localhost:5555
    // See react-native/IntegrationTests/websocket_integration_test_server.js
    // Note, the referred code has been removed in React Native 0.73
    await Facebook.React.Test.RNTesterIntegrationTests.WebSocketTest(context);
  }
  else
  {
    await DefaultRequestDelegate(context);
  }
});

// Known values:
// utf-8
// iso-8859-1
app.MapGet("/rnw/http/encoding/{charset}", async (HttpContext context, string charset) =>
{
  var response = context.Response;
  response.StatusCode = 200;

  response.ContentType = "text/plain";
  if (!string.IsNullOrEmpty(charset))
    response.ContentType += $";charset={charset}";

  // Property of Contoso–Amélior™
  var bytes = new byte[] {
    // Property of Cont
    0x50, 0x72, 0x6F, 0x70, 0x65, 0x72, 0x74, 0x79, 0x20, 0x6F, 0x66, 0x20, 0x43, 0x6F, 0x6E, 0x74,
    // oso–Amélior™
    0x6F, 0x73, 0x6F, 0xE2, 0x80, 0x93, 0x41, 0x6D, 0xC3, 0xA9, 0x6C, 0x69, 0x6F, 0x72, 0xE2, 0x84,
    0xA2
  };

  await response.Body.WriteAsync(bytes);
});

app.Map(
  "/rnw/rntester/websocketbinarytest",
  Facebook.React.Test.RNTesterIntegrationTests.WebSocketBinaryTest
  );

app.Map(
  "/rnw/websockets/echosuffix",
  Microsoft.React.Test.WebSocketTests.EchoSuffix
  );

app.MapGet(
  "/officedev/office-js/issues/4144",
   Microsoft.Office.Test.OfficeJsTests.Issue4144)
  .RequireCors(originPolicyName);

#endregion Request Mappings

await app.RunAsync();
