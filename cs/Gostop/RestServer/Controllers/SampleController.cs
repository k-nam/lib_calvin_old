using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace RestServer.Controllers {
	public class SampleController: ApiController {
		public IHttpActionResult GetSample(int id) {
			return Ok(id + 1);
		}
	}
}
