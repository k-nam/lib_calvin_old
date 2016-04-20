using Gostop.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Web.Http;
using System.Web.Script.Serialization;

namespace RestServer.Controllers {
	public class GostopController: ApiController {
		private static GostopEngine engine;

		public class Param {
			public int Value;

		}

		[HttpGet]
		public IHttpActionResult Get(int id) {
			return Ok(id + 1);
		}

		[HttpPost]
		public HttpResponseMessage PostChoice([FromUri] int action) {

			/*
			GameStep gameStep = null;
			if (action < 0) {
				engine = new GostopEngine();
				gameStep = engine.Start();
			} else {
				gameStep = engine.ProceedWith(action);
			}*/
			
			var response = new HttpResponseMessage();			
			//var json = new JavaScriptSerializer().Serialize(gameStep);
			//response.Content = new StringContent(json, Encoding.UTF8, "application/json");
			response.Content = new StringContent((action + 1).ToString());
			response.Content.Headers.ContentType = new MediaTypeHeaderValue("application/json");
			return response;
		}
	}
}
